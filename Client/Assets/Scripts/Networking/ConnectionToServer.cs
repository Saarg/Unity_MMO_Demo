using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Text;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class ConnectionToServer : MonoBehaviour {

	IPHostEntry hostEntry = null;
	[SerializeField] String ip;
	[SerializeField] int port;

	[SerializeField] bool autoconnect = true;

	Socket socket = null;
	int clientId = -1;

	[SerializeField] List<NetworkIdentity> spawnablePrefabs;
	[SerializeField] Dictionary<int, NetworkIdentity> netComps = new Dictionary<int, NetworkIdentity>();
	int idIndex = 1;

	Thread msgReceiveThread;

	String sceneName;

	void Awake()
	{
		sceneName = gameObject.scene.name;

		NetworkIdentity[] ids = FindObjectsOfType<NetworkIdentity>();
		foreach (NetworkIdentity netComp in ids) {
			if (netComp.gameObject.scene != gameObject.scene)
				continue;

			netComp.gameObject.SetActive(false);

			netComp.id = idIndex++;
			netComps.Add(idIndex, netComp);
		};

		if (autoconnect)
			Connect();
	}

	void OnDestroy() {
		Disconnect();
	}

	void Connect(NetworkIdentity player = null) {
		if (socket != null && socket.Connected) {
			Disconnect();
		}

		hostEntry = Dns.GetHostEntry(ip);
		foreach(IPAddress address in hostEntry.AddressList) {
			IPEndPoint ipe = new IPEndPoint(address, port);
			socket = new Socket(ipe.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

			socket.Connect(ipe);

			if(socket.Connected)
			{
				byte[] hello = Encoding.UTF8.GetBytes("hello from client");
				socket.Send(hello, SocketFlags.None);

				byte[] buffer = new byte[sizeof(int)];
				Read(ref buffer);
				int length = BitConverter.ToInt32(buffer, 0);

				buffer = new byte[length];
				Read(ref buffer);

				if ((MessageId) buffer[0] == MessageId.Spawn) {
					
					SpawnMessage msg = new SpawnMessage();
					msg.Deserialize(ref buffer);
					clientId = msg.objectId;

					if (player != null) {
						player.connectionToServer.Leave(player);						

						SceneManager.MoveGameObjectToScene(player.gameObject, gameObject.scene);
						player.id = msg.objectId;

						player.connectionToServer = this;

						netComps.Add(msg.objectId, player);
					} else {
						messageQueue.Enqueue(buffer);
					}
				} else {
					Debug.LogError("[" + sceneName + "] Failed to get client data");
					Disconnect();
					break;
				}

				Debug.Log("[" + sceneName + "] Client connected to " + ip + ":" + port + " with id: " + clientId + "!");
				break;
			}
			else
			{
				continue;
			}
		}

		if (!socket.Connected) {
			Debug.LogWarning("[" + sceneName + "] Failed to connect " + ip + ":" + port + "!");
			gameObject.SetActive(false);
		} else {
			foreach ( KeyValuePair<int, NetworkIdentity> netComp in netComps) {
				netComp.Value.gameObject.SetActive(true);
			}

			StartCoroutine(MsgHandling());
			
			msgReceiveThread = new Thread(new ThreadStart(MsgThread));
			msgReceiveThread.Start();
		}
	}

	void Disconnect() {
		if (socket == null || !socket.Connected) {
			return;
		}

		if (msgReceiveThread != null)
			msgReceiveThread.Abort();
		
		DespawnMessage msg = new DespawnMessage();;
		msg.objectId = clientId;

		Send(msg);

		socket.Close();

		foreach ( KeyValuePair<int, NetworkIdentity> netComp in netComps) {
			if (netComp.Value != null)
				Destroy(netComp.Value.gameObject);
		}
		netComps.Clear();

		Debug.Log("[" + sceneName + "] Disconected");
	}

	void OnTriggerEnter(Collider other)
    {
		if (other.tag != "Player" || other.gameObject.scene == gameObject.scene)
			return;

        Debug.Log("[" + sceneName + "] Entered by " + other.name);

		NetworkIdentity netId = other.GetComponent<NetworkIdentity>();

		if (netId.hasAuthority)
			Connect(netId);
    }

	void Leave(NetworkIdentity player) {
		Debug.Log("[" + sceneName + "] Left by " + player.id);
		
		netComps.Remove(player.id);
		Disconnect();
	}

	// Message handling
	Queue<byte[]> messageQueue = new Queue<byte[]>();
	IEnumerator MsgHandling() {
		while(true) {
			if (messageQueue.Count <= 0) {
				yield return null;
				continue;
			}

			byte[] buffer = messageQueue.Dequeue();
			
			if ((MessageId) buffer[0] == MessageId.Transform) {
				TransformMessage msg = new TransformMessage();
				msg.Deserialize(ref buffer);

				if (!netComps.ContainsKey(msg.sourceId)) {
					Debug.LogWarning("[" + sceneName + "] Object with netId " + msg.sourceId + " not found!");
					continue;
				}

				NetworkIdentity netId = netComps[msg.sourceId];

				msg.Apply(netId.transform);

			} else if ((MessageId) buffer[0] ==  MessageId.Spawn) {
				SpawnMessage msg = new SpawnMessage();
				msg.Deserialize(ref buffer);

				GameObject spawned = Instantiate(spawnablePrefabs[msg.prefabId].gameObject);
				SceneManager.MoveGameObjectToScene(spawned, gameObject.scene);

				NetworkIdentity netId = spawned.GetComponent<NetworkIdentity>();
				netComps.Add(msg.objectId, netId);

				netId.id = msg.objectId;
				netId.connectionToServer = this;
				netId.hasAuthority = msg.hasAuthority;
			} else if ((MessageId) buffer[0] ==  MessageId.Despawn) {
				DespawnMessage msg = new DespawnMessage();
				msg.Deserialize(ref buffer);

				if (netComps.ContainsKey(msg.objectId)) {
					NetworkIdentity netComp = netComps[msg.objectId];
					netComps.Remove(msg.objectId);

					Destroy(netComp.gameObject);
				}
			}

			yield return null;
		}
	}

	// Message reception
	void MsgThread()
	{
		while (true) {
			Thread.Sleep(10);

			byte[] buffer = new byte[sizeof(int)];
			Read(ref buffer);
			int length = BitConverter.ToInt32(buffer, 0);

			if (length <= 0) {
				continue;
			}		

			buffer = new byte[length];
			Read(ref buffer);		

			Debug.Log("[" + sceneName + "] Received message id: " + (MessageId) buffer[0]);

			messageQueue.Enqueue(buffer);
		}
	}

	public void Send(NetworkMessage msg) {		
		byte[] buffer;

		msg.Serialize(out buffer);

		Write(ref buffer);
	}


	// Socket methods to send buffers
	int Read(ref byte[] bytes, SocketFlags flags = SocketFlags.None) {
		if (socket == null || !socket.Connected)
			return 0;
		
		return socket.Receive(bytes, flags);
	}

	int Write(ref byte[] bytes, SocketFlags flags = SocketFlags.None) {
		if (socket == null || !socket.Connected)
			return 0;
		
		return socket.Send(bytes, flags);
	}
}
