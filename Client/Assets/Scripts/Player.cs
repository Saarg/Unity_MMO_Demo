using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : NetworkComponent {

    [SerializeField] string horizontalInput;
    [SerializeField] string verticalInput;

    [Range(1f, 10f)]
    [SerializeField] float speed = 5f;

    [SerializeField] Animator animator;

	void Start () {
        animator = GetComponent<Animator>();

        speed += speed * Random.Range(-0.2f, 0.2f);
    }
	
	void Update () {
        if (netId.hasAuthority) {
            Vector3 inputs = new Vector3(Input.GetAxis(horizontalInput), 0, Input.GetAxis(verticalInput)).normalized;
            Vector3 translation = inputs * Time.deltaTime * speed;

            transform.position += translation;
            if (inputs.sqrMagnitude != 0)
                transform.rotation = Quaternion.LookRotation(inputs, Vector3.up);
                        
            if (animator != null)
            {
                UpdateAnimator(translation.sqrMagnitude);
            }
            else
            {
                Debug.Log("No animator found");
            }
            
        }
	}

    private void UpdateAnimator(float speed)
    {
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
