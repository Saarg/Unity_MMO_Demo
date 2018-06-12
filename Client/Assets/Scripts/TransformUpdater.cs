using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TransformUpdater : MonoBehaviour {

    public static TransformUpdater Instance;

    private void Awake()
    {
        if(Instance == null)
        {
            Instance = this;
        }
        else
        {
            throw new System.Exception("Can't have multiple Transform Updater");
        }
    }

    // Use this for initialization
    void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    public void ApplyTransform(Transform t, Vector3 newPosition, Quaternion newRotation)
    {
        Debug.Log("Applying transform");

        Vector3 lastPosition = t.position;
        Quaternion lastRotation = t.rotation;

        Animator animator = t.GetComponent<Animator>();
        if (animator != null)
        {           
            UpdateAnimator(animator, lastPosition, newPosition);
        }
        else
        {
            Debug.Log("No animator found");
        }

        StartCoroutine( LerpTransform(t, lastPosition, newPosition, lastRotation, newRotation) );
    }

    private void UpdateAnimator(Animator animator, Vector3 lastPosition, Vector3 newPosition)
    {
        float speed = (newPosition - lastPosition).magnitude;
        Debug.Log(speed);
        if (speed > .1f)
        {
            animator.SetBool("isRunning", true);
        }
        else
        {
            animator.SetBool("isRunning", false);
        }
    }

    private IEnumerator LerpTransform(Transform t, Vector3 lastPosition, Vector3 newPosition, Quaternion lastRotation, Quaternion newRotation)
    {        
        int i = 1;
        while (i <= 10)
        {
            if (t != null)
            {
                t.position = Vector3.Lerp(lastPosition, newPosition, 0.01f * i);
                t.rotation = Quaternion.Lerp(lastRotation, newRotation, 0.01f * i);
                yield return new WaitForSeconds(.01f);
                i++;
            }
            else
            {
                break;
            }
        }      
    }

}
