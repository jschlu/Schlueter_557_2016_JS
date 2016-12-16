using UnityEngine;

public class InputManager : MonoBehaviour {
    public Transform _SolarSystem;
    public Transform _Spaceship;
    public float _SpaceshipTopSpeed = 10000;
    public Transform _RocketSpawn;
    public GameObject _RocketPrefab;
    public UnityEngine.UI.Text _SimSpeedText;
    public Transform[] _UIElements;
    public Transform _LerpTo;

    Vector3 thrustVector = new Vector3(0, 0, 0);
    Vector3 angularVelocity = new Vector3(0, 0, 0);

    private bool positionSet = false;

	// Use this for initialization
	void Start () {
        _Spaceship.position = new Vector3(16, 0, 1613);
        _Spaceship.rotation = Quaternion.Euler(0, -171.6f, 0);
    }
	
	// Update is called once per frame
	void Update () {
        // Translational Vectors
        if (Input.GetKey(KeyCode.W)) {
            if (thrustVector.z < _SpaceshipTopSpeed)
                thrustVector.z += _SpaceshipTopSpeed * 0.0005f;
        }
        else if (Input.GetKey(KeyCode.S)) {
            if (thrustVector.z > 2f)
                thrustVector.z *= 0.95f;
            else if (thrustVector.z > -_SpaceshipTopSpeed * 0.2f)
                thrustVector.z -= _SpaceshipTopSpeed * 0.0001f;
        }
        else
            thrustVector.z *= 0.985f;

        // Rotational Vectors
        if(Input.GetKey(KeyCode.A)) {
            if (angularVelocity.y > -_SpaceshipTopSpeed * 0.005f)
                angularVelocity.y -= _SpaceshipTopSpeed * 0.00003f;

            Vector3 finalRot = _Spaceship.GetChild(0).localRotation.eulerAngles;
            finalRot.z = 60;
            _Spaceship.GetChild(0).localRotation = Quaternion.Lerp(_Spaceship.GetChild(0).localRotation, Quaternion.Euler(finalRot), 2f*Time.deltaTime);
        }
        else if (Input.GetKey(KeyCode.D)) {
            if (angularVelocity.y < _SpaceshipTopSpeed * 0.005f)
                angularVelocity.y += _SpaceshipTopSpeed * 0.00003f;

            Vector3 finalRot = _Spaceship.GetChild(0).localRotation.eulerAngles;
            finalRot.z = -60;
            _Spaceship.GetChild(0).localRotation = Quaternion.Lerp(_Spaceship.GetChild(0).localRotation, Quaternion.Euler(finalRot), 2f*Time.deltaTime);
        }
        else {
            angularVelocity.y *= 0.95f;
            _Spaceship.GetChild(0).localRotation = Quaternion.Lerp(_Spaceship.GetChild(0).localRotation, Quaternion.identity, 3f*Time.deltaTime);
        }



        if(Input.GetKeyDown(KeyCode.Space)) {
            GameObject rocket = Instantiate(_RocketPrefab, _RocketSpawn.position, _RocketSpawn.rotation);
            Destroy(rocket, 10f);
        }

        _Spaceship.GetComponent<Rigidbody>().velocity = _Spaceship.TransformDirection(thrustVector);
        _Spaceship.GetComponent<Rigidbody>().angularVelocity = angularVelocity;

        transform.position = _LerpTo.position;
        transform.rotation = Quaternion.Lerp(transform.rotation, _LerpTo.rotation, Time.deltaTime * 6f);
    }

    public void OnToggleUI(UnityEngine.UI.Toggle toggle) {
        if (toggle.isOn)
            foreach (Transform element in _UIElements)
                element.gameObject.SetActive(true);
        else
            foreach (Transform element in _UIElements)
                element.gameObject.SetActive(false);
    }

    public void SetSimulationSpeed(UnityEngine.UI.Slider slider) {
        _SimSpeedText.text = slider.value.ToString();
        _SolarSystem.GetComponent<SolarSystemController>()._simSpeed = slider.value;
    }
}
