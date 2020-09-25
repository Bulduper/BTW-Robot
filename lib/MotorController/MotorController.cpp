#include "MotorController.h"

void MotorController::begin(byte stepL, byte stepR, byte dirL, byte dirR, byte enL, byte enR, bool reverse)
{
    leftStepper.init(stepL, dirL, enL, !reverse, PULSE_PERIOD_us);
    rightStepper.init(stepR, dirR, enR, reverse, PULSE_PERIOD_us);
    disable();
}
void MotorController::readyRoutine()
{
    setSpeed(0.2f);
    delay(250);
    setSpeed(0.0f);
    delay(250);
    setSpeed(-0.2f);
    delay(250);
    setSpeed(0.0f);
    delay(250);
}
void MotorController::enable()
{
    leftStepper.enable();
    rightStepper.enable();
    enabled = true;
}
void MotorController::disable()
{
    leftStepper.disable();
    rightStepper.disable();
    enabled = false;
}
bool MotorController::isEnabled()
{
    return enabled;
}
void MotorController::setSpeed(float rps)
{
    setSpeed(rps, rps);
}
void MotorController::setSpeed(float leftRPS, float rightRPS)
{
    enable();
    leftStepper.setRPS(leftRPS);
    rightStepper.setRPS(rightRPS);
}
void MotorController::run()
{
    leftStepper.run();
    rightStepper.run();
}
MotorController motors;