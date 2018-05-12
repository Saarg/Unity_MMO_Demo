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

	void Awake()
	{
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
		}
	}

	public void Send(NetworkMessage msg) {
		byte[] buffer;

		msg.Serialize(out buffer);
		Debug.Log(BitConverter.ToString(buffer));

		Write(ref buffer);
	}

	public T Receive<T>() where T : NetworkMessage {
		T msg = default(T);

		byte[] buffer = new byte[msg.Size];
		Read(ref buffer);
		Debug.Log(BitConverter.ToString(buffer));

		return msg;
	}

	int Read(ref byte[] bytes, SocketFlags flags = SocketFlags.None) {
		return socket.Receive(bytes, flags);
	}

	int Write(ref byte[] bytes, SocketFlags flags = SocketFlags.None) {
		return socket.Send(bytes, flags);
	}
}
