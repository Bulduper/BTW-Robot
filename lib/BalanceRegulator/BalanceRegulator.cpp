#include "BalanceRegulator.h"


void BalanceRegulator::setRelativeExpectedSpeed(byte relSpeed)
{
    float expSpeed = (relSpeed-50)/50.0f*MAX_SPEED_CMPS;
    setExpectedSpeed(expSpeed);
}

void BalanceRegulator::setTurnSpeed(float turnSpeed)
{
    this->turnSpeed = turnSpeed;
}

void BalanceRegulator::setRelativeTurn(byte relTurn)
{
    float turn = (relTurn-50)/50.0f*MAX_TURN_SPEED;
    setTurnSpeed(turn);
}

float BalanceRegulator::estimateSpeed(float dt)
{
    float av_cmps_vel = ((L_rps_vel + R_rps_vel) / 2.0f) * 23.25f;                    //23.25 [cm] is distanse per rotation, in cm per second
    float sensor_cmps_vel = (currentAngle - previousAngle) * DEG_TO_RAD / dt * 9.0f; //9.0f [cm] is the distance between wheel axis and MPU, in cm per second
    float estimatedSpeed = -av_cmps_vel + sensor_cmps_vel;

    static float filteredEstimatedSpeed;
    filteredEstimatedSpeed = 0.5 * filteredEstimatedSpeed + 0.5 * estimatedSpeed; //low pass filter
    return filteredEstimatedSpeed;
}

void BalanceRegulator::regulateLoop()
{
    static float prevT;

    if (!MPU6050_newData())
        return;

    MPU6050_read_3axis();
    float now = micros();
    float dt = (now - prevT) / 1000000.0f; //in seconds
    prevT = now;
    float mpuAngle = MPU6050_getAngle(dt);
    currentAngle = mpuAngle + angleOffset;

    if (fabsf(currentAngle) < WAKEUP_ANGLE)
        motors.enable();

    if (fabsf(currentAngle) > DEAD_ANGLE)
    {
        mAverageRpsVelocity = 0.0f;
        L_rps_vel = 0.0f;
        R_rps_vel = 0.0f;
        motors.disable();
    }
    if (motors.isEnabled())
    {
        float estimatedSpeed = estimateSpeed(dt);

        float targetAngle = PI_Speed(estimatedSpeed, expectedSpeed, dt);

        float regulatedSpeed = PD_Angle(currentAngle, targetAngle, dt);

        //regulatedSpeed = constrain(regulatedSpeed,-5.0f,5.0f);             //constrain the acceleration

        mAverageRpsVelocity +=regulatedSpeed;
        
        mAverageRpsVelocity = constrain(mAverageRpsVelocity,-6.0f,6.0f);

        L_rps_vel = mAverageRpsVelocity - turnSpeed;
        R_rps_vel = mAverageRpsVelocity + turnSpeed;

        motors.setSpeed(L_rps_vel, R_rps_vel);
    }
    previousAngle = currentAngle;
}

/*
    Proportional - integral regulator
    It checks the difference between the speed we want and the actual speed.
    Optionally, if the I-term !=0, any long term differences will add up causing correction.
    The output of this regulation can be perceived as the suitable, ideal angle that would result in no speed error.
*/

float BalanceRegulator::PI_Speed(float estSpeed, float expSpeed, float dt) //this is a PI regulator
{
    static float errorSum;
    float speedError = expSpeed - estSpeed;
    errorSum += speedError;
    errorSum = constrain(errorSum,-100000.0,100000.0);
    float output = speed_pid_P * speedError + speed_pid_I * errorSum * dt;

    return output;
}
/*
    Proportional - derivative regulator
    This one checks the difference between the ideal angle (from the previous regulator) and the actual angle measured by MPU.
    D - term lets the regulation proceed faster when the error is growing and slower when we are achieving the good angle.
    The output of this regulation is basically the delta speed that is later added to current motors speed causing the proper speed adjustment.
*/

float BalanceRegulator::PD_Angle(float currAngle, float targetAngle, float dt) //this is a PD regulator
{
    static float prevErrorAngle;
    float errorAngle = targetAngle - currAngle;
    float D_term = angle_pid_D * (errorAngle - prevErrorAngle) / dt;            //errorAngle-prevErrorAngle gets huge when robot falls. It falls at 50deg ang gets back at 0.5.
    D_term = constrain(D_term,-0.2f,0.2f);                                      //hence the constraint
    float output = angle_pid_P * errorAngle + D_term;
    prevErrorAngle = errorAngle;
    return output;
}