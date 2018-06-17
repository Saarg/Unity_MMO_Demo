using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : NetworkComponent {

    [SerializeField] string horizontalInput;
    [SerializeField] string verticalInput;

    [Range(1f, 10f)]
    [SerializeField] float speed = 5f;
    [Range(5f, 20f)]
    [SerializeField] float interestRadius = 8f;

    [SerializeField] new Camera camera;
    [SerializeField] Animator animator;

    [SerializeField] Renderer targetRenderer;
    [SerializeField] Material localPlayerMat;
    [SerializeField] Material onlinePlayerMat;

	void Start () {
         animator = GetComponent<Animator>();

        if (netId.hasAuthority) {
            targetRenderer.material = localPlayerMat;
        } else {
            targetRenderer.material = onlinePlayerMat;            
        }

        speed += speed * Random.Range(-0.2f, 0.2f);
    }

    void FixedUpdate() {
         if (netId.hasAuthority) {
            Vector3 inputs = new Vector3(Input.GetAxis(horizontalInput), 0, Input.GetAxis(verticalInput)).normalized;
            Vector3 translation = inputs * Time.fixedDeltaTime * speed;

            transform.position += translation;

            if (inputs.sqrMagnitude != 0) {
                Quaternion targetRot;
                targetRot = Quaternion.LookRotation(inputs, Vector3.up);

                transform.rotation = Quaternion.Lerp(transform.rotation, targetRot, 3f*Time.fixedDeltaTime);
            }
            
            if (animator != null)
            {
                UpdateAnimator(translation.sqrMagnitude);
            }
            else
            {
                Debug.Log("No animator found");
            }
        } else if (camera != null) {
            Destroy(camera.gameObject);
        }
    }

    private void UpdateAnimator(float speed)
    {
        if (speed > 0f)
        {
            animator.SetBool("isRunning", true);
        }
        else
        {
            animator.SetBool("isRunning", false);
        }
    }

    void OnDrawGizmos() {
        Gizmos.color = Color.green;
        Gizmos.DrawWireSphere(transform.position, interestRadius);
    }
}
