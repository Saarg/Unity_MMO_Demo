using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : NetworkComponent {

    [SerializeField] string horizontalInput;
    [SerializeField] string verticalInput;
    [SerializeField] Animator animator;

	void Start () {
        animator = GetComponent<Animator>();
    }
	
	void Update () {
        if (netId.hasAuthority) {
            Vector3 inputs = new Vector3(Input.GetAxis(horizontalInput), 0, Input.GetAxis(verticalInput)).normalized;
            Vector3 translation = inputs * Time.deltaTime * 5f;

            transform.position += translation;
            transform.rotation = Quaternion.LookRotation(inputs, Vector3.up);
                        
            if (animator != null)
            {
                UpdateAnimator(translation.magnitude);
            }
            else
            {
                Debug.Log("No animator found");
            }
            
        }
	}

    private void UpdateAnimator(float speed)
    {
        Debug.Log(speed);
        if (speed > .01f)
        {
            animator.SetBool("isRunning", true);
        }
        else
        {
            animator.SetBool("isRunning", false);
        }
    }
}
