using UnityEngine;

public class Explode : MonoBehaviour {
    public GameObject _ExplosionEffect;


    private ParticleCollisionEvent[] collisionEvents = new ParticleCollisionEvent[16];
    void OnParticleCollision(GameObject other){
        int safeLength = GetComponent<ParticleSystem>().GetSafeCollisionEventSize();
        if (collisionEvents.Length < safeLength)
            collisionEvents = new ParticleCollisionEvent[safeLength];
        int numCollisionEvents = GetComponent<ParticleSystem>().GetCollisionEvents(other, collisionEvents);
        int i = 0;
        while (i < numCollisionEvents) {
            Vector3 collisionHitLoc = collisionEvents[i].intersection;
            GameObject explosion = Instantiate(_ExplosionEffect, collisionHitLoc, Quaternion.identity) as GameObject;
            Destroy(explosion, 10f);
            i++;
        }
        if (other.CompareTag("Asteroid"))
            Destroy(other);
    }
}
