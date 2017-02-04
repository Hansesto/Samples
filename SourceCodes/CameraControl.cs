using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraControl : MonoBehaviour {

    public Transform player;
    public float cameraHeight;
    public float distance;

    private Vector3 offset;
    private Vector3 lastStablePosition;
    private float minVelocity;

	// Use this for initialization
	void Start () {

        //Set the minimum velocity, when camera starts following player
        minVelocity = 4;

        //Set up the initial stable camera position (player hasn't moved yet)
        offset = new Vector3(0, 5, -10);
        lastStablePosition = player.position + offset;

        //Set camera's position to last stable positiot (initial value)
        transform.position = lastStablePosition;
        transform.LookAt(player);
	}
	
	// Update is called once per frame
	void LateUpdate () {

        //Store player's velocity components
        float[] velocities = new float[3];
        velocities[0] = player.GetComponent<Rigidbody>().velocity.x;
        velocities[1] = player.GetComponent<Rigidbody>().velocity.y;
        velocities[2] = player.GetComponent<Rigidbody>().velocity.z;

        //DEBUG
        if (Input.GetKey(KeyCode.Space))
        {
            print(velocities[0]);
            print(velocities[1]);
            print(velocities[2]);
        }

        //Calculate the length of the player's velocity vector
        float velocityVectorLength = Mathf.Sqrt(Mathf.Pow(velocities[0], 2) + Mathf.Pow(velocities[1], 2) + Mathf.Pow(velocities[2], 2));

        //Make adjustments to camera if player's velocity exceeds the limit
        if (velocityVectorLength >= minVelocity)
        {
            //Normalize the velocity vector
            for (int i = 0; i < velocities.Length; ++i)
            {
                velocities[i] = velocities[i] / velocityVectorLength;
            }

            //Calculate cameras position relative to players position on a line defined by
            //player's position and velocity's (normalized) direction
            Vector3 followDirection = new Vector3(velocities[0], velocities[1], velocities[2]);
            Vector3 cameraPosition = player.position + distance * followDirection;

            //Set the camera's position (+ offset height) and look at player
            transform.position = cameraPosition + new Vector3(0, cameraHeight, 0);
            transform.LookAt(player);

            //Refresh last stable position
            lastStablePosition = transform.position;
        }
        else
        {
            //Keep the last stable position for the camera
            transform.position = lastStablePosition;
            transform.LookAt(player);
        }
    }
}
