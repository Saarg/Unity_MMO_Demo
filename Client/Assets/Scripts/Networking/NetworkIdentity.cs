using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NetworkIdentity : MonoBehaviour {

	public int id;
	public ConnectionToServer connectionToServer;
	public bool spawned = true;

	public bool hasAuthority = false;
	public bool HasAuthority { get { return HasAuthority; } }

	public virtual void OnConnect(ConnectionToServer connection) {
		connectionToServer = connection;
	}

	public virtual void OnDisconnect() {
		connectionToServer = null;

		if (spawned)
			Destroy(gameObject);
	}
}
