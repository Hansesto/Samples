using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BallCtrl : MonoBehaviour
{

    public float dragSpeed;
    public float fwdVelScale;
    public float maxVel = 1;

    public float rotBrakeDrag;
    public float maxRotSpeed;
    public float rotForce;
    public float rotThold = 7;
    public float restRotThold = 1;

    public float enghRotForCrve;
    public float minCurve = 1;
    public float midCurve = 1;
    public float maxCurve = 1;

    public int curvePoints = 1;
    public int bouncePoints = 1;

    private AnimationManager animCtrlr;

    private Rigidbody rb;
    private ConstantForce cForce;
    private Vector3 tarScrnPos;
    private Vector3 startPosition;
    //private Vector3 offset;

    private Quaternion startRotation;
    private Vector3[] prevVels;
    private float curveDir;
    private bool thrown;

    private bool canScore;
    private bool canBullsEye;
    private int bounceTimes;

    // Use this for initialization
    void Start()
    {
        rb = gameObject.GetComponent<Rigidbody>();
        rb.maxAngularVelocity = maxRotSpeed;
        rb.isKinematic = true;
        cForce = gameObject.GetComponent<ConstantForce>();

        startPosition = transform.position;
        startRotation = transform.rotation;

        prevVels = new Vector3[10];
        for (int i = 0; i < prevVels.Length; i++)
        {
            prevVels[i] = Vector3.zero;
        }

        thrown = false;
        canScore = false;
        canBullsEye = true;
        bounceTimes = 0;

        animCtrlr = GameObject.FindGameObjectWithTag("AnimationEffects").GetComponent<AnimationManager>();
    }

    private void Update()
    {
        if (Input.GetKey(KeyCode.D))
        {
            animCtrlr.playAnimation("BounceNoticed");
        }
        else if (Input.GetKey(KeyCode.S))
        {
            animCtrlr.playAnimation("CurveNoticed");
        }
        else if (Input.GetKey(KeyCode.A))
        {
            animCtrlr.playAnimation("BullsEye");
        }
    }

    //Calculate scores
    private void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.name == "Trigger0")
        {
            canScore = true;
        }
        else if (other.gameObject.name == "Trigger1")
        {
            if (canScore)
            {
                //Score detected
                ScoreManager.sessionScore += 1 + (int)Mathf.Abs(curveDir) * curvePoints + bounceTimes * bouncePoints;
                if (curveDir != 0)
                {
                    ScoreManager.curveGoals += 1;
                    animCtrlr.playAnimation("CurveNoticed");
                }
                else if (bounceTimes > 0)
                {
                    animCtrlr.playAnimation("BounceNoticed");
                    ScoreManager.bounceGoals += 1;
                }
                else if (canBullsEye)
                {
                    animCtrlr.playAnimation("BullsEye");
                    ScoreManager.bullsEyes += 1;
                }

                ScoreManager.allGoals += 1;
                canScore = false;
            }
        }
    }

    private void OnMouseDown()
    {
        if (!thrown)
        {
            //Ball's position relative to screen coordinates
            tarScrnPos = Camera.main.WorldToScreenPoint(rb.transform.position);
            //offset = rb.transform.position - Camera.main.ScreenToWorldPoint(new Vector3(Input.mousePosition.x, Input.mousePosition.y, tarScrnPos.z));

        }
    }

    private void OnMouseDrag()
    {
        if (!thrown)
        {
            rb.angularDrag = 0.05F;

            rb.isKinematic = false;

            //Unfreeze rotation
            rb.freezeRotation = false;

            //Constrain the ball's rotation along every other but z-axis
            //rb.constraints = RigidbodyConstraints.FreezeRotationX | RigidbodyConstraints.FreezeRotationY;

            //Translate cursor's position to world position
            Vector3 touchPoint = new Vector3(Input.mousePosition.x, Input.mousePosition.y, tarScrnPos.z);
            Vector3 touchPos = Camera.main.ScreenToWorldPoint(touchPoint) /* + offset */ ;

            //Change ball's velocity according to mouse movement
            rb.velocity = (touchPos - rb.transform.position) * dragSpeed;

            //Check that throwing velocity doesn't exceed the predefined max value 
            if (rb.velocity.sqrMagnitude > (maxVel * maxVel))
            {
                rb.velocity = rb.velocity.normalized * maxVel;
            }

            //Detect curvemotion and get the rotation direction
            float currRotDir = curveMotion();

            //apply rotation if curvemotion detected
            if (currRotDir != 0)
            {
                changeRotation(currRotDir);
                curveDir = currRotDir;
            }
            else //No curve motion
            {
                //Slow down the rotation if not in acceptable levels
                if (rb.angularVelocity.sqrMagnitude > Mathf.Pow(restRotThold, 2))
                {
                    //changeRotation(-prevRotDir * brkScale);
                    rb.angularDrag = rotBrakeDrag;
                }
                else //Ball at rest or within rest rotation treshold limits
                {
                    currRotDir = curveDir = 0;
                }
            }

            //Shift the previous velocities array so the new value fits in
            Vector3[] temp = new Vector3[prevVels.Length];
            System.Array.ConstrainedCopy(prevVels, 0, temp, 1, prevVels.Length - 1);
            prevVels = temp;

            //Add the current velocity to previous velocities
            prevVels[0] = rb.velocity;
        }
    }

    private void OnMouseUp()
    {
        if (!thrown)
        {
            //Free the ball from rotation constraints
            rb.constraints = RigidbodyConstraints.None;
            rb.angularDrag = 0.05F;

            //Check that throwing velocity doesn't exceed the predefined max value 
            if (rb.velocity.sqrMagnitude > (maxVel * maxVel))
            {
                rb.velocity = rb.velocity.normalized * maxVel;
            }

            //Add the forward velocity to ball
            rb.velocity = new Vector3(rb.velocity.x, rb.velocity.y, rb.velocity.magnitude * fwdVelScale);


            //Check if the ball is spinning enough for a curve to be applied
            if (rb.angularVelocity.sqrMagnitude > Mathf.Pow(enghRotForCrve, 2))
            {
                float angle = Vector3.Angle(rb.velocity, Vector3.up);
                float minAngle = 37;
                float maxAngle = 50;

                if (angle < minAngle)
                {
                    //Apply min curveForce to the flying ball
                    cForce.force = Vector3.left * curveDir * minCurve;
                }
                else if (angle > maxAngle)
                {
                    //Apply max curveForce to the flying ball
                    cForce.force = Vector3.left * curveDir * maxCurve;
                }
                else
                {
                    //Apply medium curveForce (depending on the throwing angle) to the flying ball
                    cForce.force = Vector3.left * curveDir * (angle / maxAngle) * midCurve;
                }

                ScoreManager.curveThrows += 1;
            }
            else
            {
                //Not enough curve
                curveDir = 0;
            }

            //Ball thrown
            thrown = true;
            ScoreManager.throws += 1;
        }
    }

    public void resetBall()
    {
        if (BallSceneMngr.currentItem == gameObject)
        {
            transform.position = startPosition;
            transform.rotation = startRotation;
            rb.velocity = Vector3.zero;
            rb.angularDrag = 0.05F;
            rb.freezeRotation = true;
            curveDir = 0;
            rb.isKinematic = true;
            rb.constraints = RigidbodyConstraints.None;

            for (int i = 0; i < prevVels.Length; i++)
            {
                prevVels[i] = Vector3.zero;
            }

            cForce.force = Vector3.zero;
            thrown = false;
            canScore = false;
            canBullsEye = true;
            bounceTimes = 0;
        } 
    }

    private void changeRotation(float rotDir)
    {
        //Move this to setup function as this is refined enough
        rb.maxAngularVelocity = maxRotSpeed;

        //Apply rotDir-direction (=0 if no curvemotion) torque to ball to increase it's angular velocity
        rb.AddTorque(Vector3.forward * rotForce * rotDir);
    }

    private float curveMotion()
    {
        //Check angle between current velocity and last previous velocity
        if (!(Vector3.Angle(rb.velocity, prevVels[0]) > rotThold))
        {
            //No curve motion
            return 0F;

        }
        else
        {
            for (int i = 0; i < prevVels.Length - 1; i++)
            {
                //Check angles between previous velocities
                if (!(Vector3.Angle(prevVels[i], prevVels[i + 1]) > rotThold))
                {
                    //No curve motion
                    return 0F;
                }
            }
        }

        float rotDir = (Vector3.Cross(rb.velocity, prevVels[0]).z > 0) ? -1F : 1F;

        //Curve motion detected, returns 1 if CW, -1 if CCW
        return rotDir;
    }

    private void OnCollisionEnter(Collision collision)
    {
        //Ball hit something, cancel curving force
        cForce.force = Vector3.zero;

        if (collision.gameObject.name == "Court")
        {
            bounceTimes += 1;
        }
        else
        {
            canBullsEye = false;
        }
    }
}
