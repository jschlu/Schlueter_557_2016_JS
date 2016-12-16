using UnityEngine;

public class PlanetController : MonoBehaviour {
    [Tooltip("Diameter of planet (in miles)")]
    public float _diameter;
    [Tooltip("Distance from the sun (in million miles)")]
    public float _distanceFromOrbitalBody;

    [Tooltip("Rotation period of planet (in hours)")]
    public float _rotationPeriod;
    [Tooltip("Orbital period of planet (in days)")]
    public float _orbitalPeriod;

    [Tooltip("Axes offset of planet (in degrees)")]
    public float _axisOffset;

    public bool _isMoon, _isSun;

    public SolarSystemController masterScript;

    // Use this for initialization
    void Start () {
        _orbitalPeriod = _orbitalPeriod * 24.0f;
        transform.localScale = masterScript._sizeMult * new Vector3(_diameter, _diameter, _diameter);
        transform.Rotate(_axisOffset, 0, 0, Space.Self);


        if(_isSun)
            transform.localPosition = masterScript._distanceMult * new Vector3(0, 0, _distanceFromOrbitalBody);
        else if(!_isMoon)
            transform.localPosition = masterScript._distanceMult * new Vector3(0, 0, _distanceFromOrbitalBody + masterScript._offset);
        else
            transform.localPosition = masterScript._distanceMult * new Vector3(0, 0, _distanceFromOrbitalBody);
    }
	
	// Update is called once per frame
	void Update () {
        if(!_isSun)
            transform.RotateAround(transform.parent.position, Vector3.up, masterScript._simSpeed / _orbitalPeriod);
        transform.Rotate(0, 0, masterScript._simSpeed / _rotationPeriod, Space.Self);
	}
}
