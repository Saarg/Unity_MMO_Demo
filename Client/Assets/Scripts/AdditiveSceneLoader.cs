using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class AdditiveSceneLoader : MonoBehaviour {

	[SerializeField] int[] scenes;

	void Awake () {
		#if !UNITY_EDITOR
		StartCoroutine(LoadAllScenes());
		#endif
	}

	IEnumerator LoadAllScenes() {
		foreach (int sceneIndex in scenes)
		{

			if (SceneManager.GetSceneByBuildIndex(sceneIndex).isLoaded)
				continue;

			AsyncOperation asyncLoad = SceneManager.LoadSceneAsync(sceneIndex, LoadSceneMode.Additive);

			while (!asyncLoad.isDone)
			{
				yield return null;
			}
		}
	}
}
