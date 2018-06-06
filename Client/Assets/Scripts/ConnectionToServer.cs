using System;
using System.Net;
using System.Net.Sockets;
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

			StartCoroutine(MsgCoroutine());

			// GameObject p = Instantiate(player.gameObject, new Vector3(-5f, 0.5f, 0f), Quaternion.identity);
			// NetworkIdentity netId = p.GetComponent<NetworkIdentity>();
			// netId.hasAuthority = true;

			// netComps.Add(p.GetComponent<NetworkIdentity>());

			// SpawnMessage msg = new SpawnMessage(p.transform);
			// msg.prefabId = spawnablePrefabs.Count-1;
			// msg.objectId = netComps.Count-1;

			// Send(msg);
		}
	}

	// Message handling
	IEnumerator MsgCoroutine()
	{
		while (true) {
			byte[] buffer = new byte[sizeof(int)];
			Read(ref buffer);
			int length = BitConverter.ToInt32(buffer, 0);

			if (length <= 0) {
				yield return null;
				continue;
			}		

			byte[] id = new byte[1];
			Read(ref id);

			buffer = new byte[length];
			Read(ref buffer);				
			
			if ((MessageId) id[0] == MessageId.Transform) {
				TransformMessage msg = new TransformMessage();
				msg.Deserialize(ref buffer);			

			} else if ((MessageId) id[0] ==  MessageId.Spawn){
				SpawnMessage msg = new SpawnMessage();
				msg.Deserialize(ref buffer);

				Debug.Log(msg.prefabId);
				GameObject spawned = Instantiate(spawnablePrefabs[msg.prefabId].gameObject);

				NetworkIdentity netId = spawned.GetComponent<NetworkIdentity>();
				netComps.Add(netId);

				netId.id = msg.objectId;
			}

			yield return null;
		}
	}

	public void Send(NetworkMessage msg) {
		byte[] buffer;

		msg.Serialize(out buffer);
		Debug.Log(BitConverter.ToString(buffer));

		Write(ref buffer);
	}

	int Read(ref byte[] bytes, SocketFlags flags = SocketFlags.None) {
		return socket.Receive(bytes, flags);
	}

	int Write(ref byte[] bytes, SocketFlags flags = SocketFlags.None) {
		return socket.Send(bytes, flags);
	}
}
