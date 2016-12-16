using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AsteroidSpawner : MonoBehaviour {
    public GameObject AsteroidPrefab;

	// Use this for initialization
	void Start () {
        Invoke("SpawnAsteroid", Random.Range(1f, 5f));
	}

    void SpawnAsteroid() {
        float randFloat = Random.Range(25f, 100f);
        Quaternion randRot = Quaternion.Euler(0, Random.Range(0f, 360f), 0);
        transform.rotation = randRot;
        transform.Translate(Vector3.forward * randFloat, Space.Self);

        randRot = Quaternion.Euler(Random.Range(0f, 360f), Random.Range(0f, 360f), Random.Range(0f, 360f));
        GameObject asteroid = Instantiate(AsteroidPrefab, transform.position, randRot);
        randFloat = Random.Range(0.3f, 6f);
        asteroid.transform.localScale = new Vector3(randFloat, randFloat, randFloat);

        asteroid.GetComponent<AsteroidMover>().MoveVector = new Vector3(Random.Range(0.1f, 2f), 0, Random.Range(0.1f, 2f));
        asteroid.GetComponent<AsteroidMover>().RotationVector = new Vector3(Random.Range(0.05f, 3f), Random.Range(0.05f, 3f), Random.Range(0.05f, 3f));

        transform.localPosition = new Vector3(0, 0, 0);

        Invoke("SpawnAsteroid", Random.Range(1f, 5f));
    }
}
