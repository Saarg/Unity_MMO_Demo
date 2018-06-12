using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(NetworkIdentity))]
public class NetworkComponent : MonoBehaviour {

	protected NetworkIdentity netId;

	public bool HasAuthority { get { return netId.HasAuthority; } }

	void Awake()
	{
		netId = GetComponent<NetworkIdentity>();
	}

	public virtual void OnConnect(ConnectionToServer connection) {}
	public virtual void OnDisconnect() {}
}
