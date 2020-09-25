#include "Motor.h"

void Motor::init(byte step, byte dir, byte en, bool side, uint16_t pulse_T)
{
    stepPin = step;
    dirPin = dir;
    enPin = en;
    this->side = side;
    pulsePeriod_us = pulse_T;

    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(enPin, OUTPUT);
}

void Motor::enable()
{
    digitalWrite(enPin, LOW);
}

void Motor::disable()
{
    digitalWrite(enPin, HIGH);
}

void Motor::run()
{
    pulseCounter++;
    if (pulseCounter > currentPulseInterval)
    {
        digitalWrite(dirPin, currentDirection ^ side); //set direction of rotation on dirPin according to what motor it is
        pulseCounter = 0;
    }
    else if (pulseCounter == 1) //create a step signal
    {
        digitalWrite(stepPin, HIGH);
    }
    else if (pulseCounter == 2)
    {
        digitalWrite(stepPin, LOW);
    }
}

void Motor::setRPS(float speed_rps)
{
    float speed_rps_abs = fabsf(speed_rps);
    if (speed_rps_abs < 0.001f)
    {
        currentPulseInterval = 0;
        return;
    }

    currentDirection = (speed_rps > 0 ? FORWARD : BACKWARD);
    currentPulseInterval = 5000.0f / (float)pulsePeriod_us / (float)MICROSTEPS / speed_rps_abs;
}