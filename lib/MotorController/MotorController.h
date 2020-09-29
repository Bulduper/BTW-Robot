#ifndef _MOTOR_CONTROLLER_H
#define _MOTOR_CONTROLLER_H

#include <Arduino.h>
#include <Motor.h>

#define PULSE_PERIOD_us 16

class MotorController
{
    private:
    bool enabled = false;
    Motor leftStepper, rightStepper;

    public:
    void begin(byte stepL, byte stepR, byte dirL, byte dirR, byte enL, byte enR, bool reverse);
    void readyRoutine();
    void enable();
    void disable();
    bool isEnabled();
    void setSpeed(float rps);
    void setSpeed(float leftRPS, float rightRPS);
    void run();

    float getRPulseInterval() {return rightStepper.desiredStepInterval;}

};
extern MotorController motors;
#endif