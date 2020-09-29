#ifndef _BALANCE_REGULATOR_H
#define _BALANCE_REGULATOR_H
#include <Arduino.h>
#include <mpu6050.h>
#include <MotorController.h>
#include <WirelessData.h>

#define speed_P_term 0.2f //0.2 is great
#define speed_I_term 0.0f
#define angle_P_term 0.005f //0.005 is great
#define angle_D_term 0.001f //0.001 is great

#define ANGLE_OFFSET 1.6f
#define MAX_SPEED_CMPS 100.0f
#define MAX_TURN_SPEED 1.0f

#define DEAD_ANGLE 50.0f
#define WAKEUP_ANGLE 1.0f

//TO DO:
//Make offset angle editable through WiFi controller

class BalanceRegulator
{
private:
    float speed_pid_P = speed_P_term, speed_pid_I = speed_I_term;
    float angle_pid_P = angle_P_term, angle_pid_D = angle_D_term;
    float currentAngle, previousAngle;
    float mAverageRpsVelocity;
    float R_rps_vel, L_rps_vel;
    float angleOffset = ANGLE_OFFSET;
    float expectedSpeed = 0.0f;
    float turnSpeed = 0.0f;

    float estimateSpeed(float);
    float PI_Speed(float, float, float);
    float PD_Angle(float, float, float);

public:
    float getLeftVelocity() { return L_rps_vel; }
    float getRightVelocity() { return R_rps_vel; }
    float getCurrentAngle() { return currentAngle; }
    float getSpeedPidP() { return speed_pid_P; }
    float getSpeedPidI() { return speed_pid_I; }
    float getAnglePidP() { return angle_pid_P; }
    float getAnglePidD() { return angle_pid_D; }

    void setLeftVelocity(float vel){L_rps_vel = vel;}
    void setRightVelocity(float vel){R_rps_vel = vel;}
    void setSpeedPidP(float p) { speed_pid_P = p; }
    void setSpeedPidI(float i) { speed_pid_I = i; }
    void setAnglePidP(float p) { angle_pid_P = p; }
    void setAnglePidD(float d) { angle_pid_D = d; }
    void setAngleOffset(float o) { angleOffset = o; }
    void setExpectedSpeed(float e) { expectedSpeed = e; }
    void setRelativeExpectedSpeed(byte);
    void setTurnSpeed(float s);
    void setRelativeTurn(byte);

    void regulateLoop();
};
extern BalanceRegulator Regulator;
#endif