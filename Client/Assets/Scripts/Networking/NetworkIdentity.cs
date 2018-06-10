using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NetworkIdentity : MonoBehaviour {

	public int id;
	public ConnectionToServer connectionToServer;

	public bool hasAuthority = false;
	public bool HasAuthority { get { return HasAuthority; } }
}
