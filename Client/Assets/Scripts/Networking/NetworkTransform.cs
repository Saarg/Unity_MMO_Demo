using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NetworkTransform : NetworkComponent {

	float lastUpdate;
	[Range(0f, 30f)]
	[SerializeField] float updatePerSec = 10f;

	float lastReception;

	Vector3 lastPosition;
	Quaternion lastRotation;

	Vector3 newPosition;
	Quaternion newRotation;

	// Use this for initialization
	void Start () {
		lastUpdate = Time.realtimeSinceStartup;

		lastPosition = newPosition = transform.position;
		lastRotation = newRotation = transform.rotation;
	}
	
	// Update is called once per frame
	void Update () {
		if (netId.connectionToServer != null && netId.hasAuthority && updatePerSec != 0 && Time.realtimeSinceStartup - lastUpdate > 1 / updatePerSec) {
			lastUpdate = Time.realtimeSinceStartup;			
			TransformMessage message = new TransformMessage(transform);

			netId.connectionToServer.Send(message);
		}

		if (!netId.hasAuthority) {
			float lerpValue = Mathf.Clamp01((Time.realtimeSinceStartup - lastReception) / (1 / updatePerSec));

			transform.position = Vector3.Lerp(lastPosition, newPosition, lerpValue);
			transform.rotation = Quaternion.Lerp(lastRotation, newRotation, lerpValue);
		}
	}


	public void ApplyTransform(TransformMessage msg)
    {
        newPosition = new Vector3(msg.position[0], msg.position[1], msg.position[2]);
        newRotation = new Quaternion(msg.rotation[0], msg.rotation[1], msg.rotation[2], msg.rotation[3]);

        lastPosition = transform.position;
        lastRotation = transform.rotation;

        Animator animator = transform.GetComponent<Animator>();
        if (animator != null)
        {           
            UpdateAnimator(animator, lastPosition, newPosition);
        }
        else
        {
            Debug.Log("No animator found");
        }

		lastReception = Time.realtimeSinceStartup;
    }

    private void UpdateAnimator(Animator animator, Vector3 lastPosition, Vector3 newPosition)
    {
        float speed = (newPosition - lastPosition).sqrMagnitude;

        if (speed > .1f)
        {
            animator.SetBool("isRunning", true);
        }
        else
        {
            animator.SetBool("isRunning", false);
        }
    }

    private IEnumerator LerpTransform(Vector3 lastPosition, Vector3 newPosition, Quaternion lastRotation, Quaternion newRotation)
    {        
        float startTime = Time.realtimeSinceStartup;
        while (Time.realtimeSinceStartup - startTime <= 1 / updatePerSec)
        {
			float lerpValue = Mathf.Clamp01((Time.realtimeSinceStartup - startTime) / (1 / updatePerSec));

			transform.position = Vector3.Lerp(lastPosition, newPosition, lerpValue);
			transform.rotation = Quaternion.Lerp(lastRotation, newRotation, lerpValue);
			yield return null;
        }      
    }
}
