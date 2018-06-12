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

    public void ApplyTransform(Transform t, Vector3 v, Quaternion q)
    {
        StartCoroutine( LerpTransform(t,v,q) );
    }

    private IEnumerator LerpTransform(Transform t, Vector3 v, Quaternion q)
    {
        int i = 1;
        Vector3 lastFramePosition = t.position;
        Quaternion lastFrameRotation = t.rotation;
        while (i <= 10)
        {
            t.position = Vector3.Lerp(lastFramePosition, v, 0.1f * i);
            t.rotation = Quaternion.Lerp(lastFrameRotation, q, 0.1f * i);
            yield return new WaitForSeconds(.01f);
            i++;
        }      
    }

}
