using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Text;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ConnectionToServer : MonoBehaviour {

	IPHostEntry hostEntry = null;
	[SerializeField] String ip;
	[SerializeField] int port;

	Socket socket = null;
	int clientId = -1;

	[SerializeField] List<NetworkIdentity> spawnablePrefabs;

	[SerializeField] List<NetworkIdentity> netComps = new List<NetworkIdentity>();
	int idIndex = 1;

	void Awake()
	{
		netComps.AddRange(FindObjectsOfType<NetworkIdentity>());
		netComps.ForEach((netComp) => {
			netComp.gameObject.SetActive(false);

			netComp.id = idIndex++;
		});

		Connect();
	}

	void Connect() {
		hostEntry = Dns.GetHostEntry(ip);
		foreach(IPAddress address in hostEntry.AddressList) {
			IPEndPoint ipe = new IPEndPoint(address, port);
			socket = new Socket(ipe.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

			socket.Connect(ipe);

			if(socket.Connected)
			{
				byte[] msg = Encoding.UTF8.GetBytes("hello from client");

				socket.Send(msg, SocketFlags.None);

				byte[] bytes = new byte[sizeof(int)];
				if (socket.Receive(bytes, SocketFlags.None) > 0)
					clientId = BitConverter.ToInt32(bytes, 0);
				else
					Debug.LogError("Failed to get client id");

				Debug.Log("Client connected to " + ip + ":" + port + "!");
				break;
			}
			else
			{
				continue;
			}
		}

		if (!socket.Connected) {
			Debug.LogWarning("Failed to connect " + ip + ":" + port + "!");
			gameObject.SetActive(false);
		} else {
			netComps.ForEach((netComp) => {
				netComp.gameObject.SetActive(true);
			});

			StartCoroutine(MsgHandling());
			
			Thread t = new Thread(new ThreadStart(MsgThread));
			t.Start();
		}
	}

	Queue<byte[]> messageQueue = new Queue<byte[]>();
	IEnumerator MsgHandling() {
		while(true) {
			if (messageQueue.Count <= 0) {
				yield return new WaitForSeconds(0.1f);
				continue;
			}

			byte[] buffer = messageQueue.Dequeue();
			
			if ((MessageId) buffer[0] == MessageId.Transform) {
				TransformMessage msg = new TransformMessage();
				msg.Deserialize(ref buffer);

				NetworkIdentity netId = netComps.Find(item => item.id == msg.sourceId);
				msg.Apply(netId.transform);

			} else if ((MessageId) buffer[0] ==  MessageId.Spawn){
				SpawnMessage msg = new SpawnMessage();
				msg.Deserialize(ref buffer);

				GameObject spawned = Instantiate(spawnablePrefabs[msg.prefabId].gameObject);

				NetworkIdentity netId = spawned.GetComponent<NetworkIdentity>();
				netComps.Add(netId);

				netId.id = msg.objectId;
				netId.hasAuthority = msg.hasAuthority;
			}

			yield return null;
		}
	}

	// Message reception
	void MsgThread()
	{
		while (true) {
			byte[] buffer = new byte[sizeof(int)];
			Read(ref buffer);
			int length = BitConverter.ToInt32(buffer, 0);

			if (length <= 0) {
				continue;
			}		

			buffer = new byte[length + 1];
			Read(ref buffer);		

			Debug.Log("Received message id: " + (MessageId) buffer[0]);

			messageQueue.Enqueue(buffer);
		}
	}

	public void Send(NetworkMessage msg) {
		byte[] buffer;

		msg.Serialize(out buffer);

		Write(ref buffer);
	}

	int Read(ref byte[] bytes, SocketFlags flags = SocketFlags.None) {
		return socket.Receive(bytes, flags);
	}

	int Write(ref byte[] bytes, SocketFlags flags = SocketFlags.None) {
		return socket.Send(bytes, flags);
	}
}
