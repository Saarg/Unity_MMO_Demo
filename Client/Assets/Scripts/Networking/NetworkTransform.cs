using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NetworkTransform : NetworkComponent {

	float lastUpdate;
	[Range(0f, 30f)]
	[SerializeField] float updatePerSec = 10f;
	ConnectionToServer connectionToServer = null;

	// Use this for initialization
	void Start () {
		foreach (ConnectionToServer connection in FindObjectsOfType<ConnectionToServer>())
		{
			if (connection.transform.root == transform.root) {
				connectionToServer = connection;
				break;
			}
		}
		connectionToServer = FindObjectOfType<ConnectionToServer>();

		lastUpdate = Time.realtimeSinceStartup;
	}
	
	// Update is called once per frame
	void Update () {
		if (connectionToServer != null && netId.hasAuthority && updatePerSec != 0 && Time.realtimeSinceStartup - lastUpdate > 1 / updatePerSec) {
			lastUpdate = Time.realtimeSinceStartup;			
			TransformMessage message = new TransformMessage(transform);

			connectionToServer.Send(message);
		}
	}
}
