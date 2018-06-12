using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NetworkTransform : NetworkComponent {

	float lastUpdate;
	[Range(0f, 30f)]
	[SerializeField] float updatePerSec = 10f;

	// Use this for initialization
	void Start () {
		lastUpdate = Time.realtimeSinceStartup;
	}
	
	// Update is called once per frame
	void Update () {
		if (netId.connectionToServer != null && netId.hasAuthority && updatePerSec != 0 && Time.realtimeSinceStartup - lastUpdate > 1 / updatePerSec) {
			lastUpdate = Time.realtimeSinceStartup;			
			TransformMessage message = new TransformMessage(transform);

			netId.connectionToServer.Send(message);
		}
	}
}
