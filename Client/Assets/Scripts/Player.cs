using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : NetworkComponent {

    [SerializeField] string horizontalInput;
    [SerializeField] string verticalInput;

	void Start () {
		
	}
	
	void Update () {
        if (netId.hasAuthority) {
            transform.Translate(new Vector3(Time.deltaTime * Input.GetAxis(horizontalInput), 0, Time.deltaTime * Input.GetAxis(verticalInput)));
        }
	}
}
