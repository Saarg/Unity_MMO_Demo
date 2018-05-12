using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(NetworkIdentity))]
public class NetworkComponent : MonoBehaviour {

	protected NetworkIdentity netId;
	void Awake()
	{
		netId = GetComponent<NetworkIdentity>();
	}

}
