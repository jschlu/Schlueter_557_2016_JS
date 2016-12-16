using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AsteroidMover : MonoBehaviour {
    public Vector3 MoveVector;
    public Vector3 RotationVector;

	// Use this for initialization
	void Start () {
        GetComponent<Rigidbody>().velocity = MoveVector;
        GetComponent<Rigidbody>().angularVelocity = RotationVector;
	}
}
