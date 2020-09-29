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

/*
We can perform the rotation in steps that are made only in fixed intervals (i.e. 30us, 40us, 50us)
So for a specific range of rational RPS values we want to achieve, the motors would actually spin at the constant speed, 
eventually causing the whole values jumping back and forth and bad oscillations for higher speeds.
The solution is varying the step intervals so that they are rational in average (and close to the desired value).
In the result no oscillation occur and we may even increase the PULSE_PERIOD.
run() and setRPS() methods implement that solution.
For instace:
- if the desiredStepInterval equals 8.2 then every 5th step will be performed after 9th (and not 8th) pulse
- or if the desiredStepInterval equals 11.9 then 9 out of 10 steps will be performed after 12th (and not 11th) pulse
*/

void Motor::run()
{
    pulseCounter++;
    if (pulseCounter > currentStepPulse - previousStepPulse)
    {
        digitalWrite(dirPin, currentDirection ^ side); //set direction of rotation on dirPin according to what motor it is
        pulseCounter = 0;

        previousStepPulse = currentStepPulse;
        currentStepPulse += desiredStepInterval;    
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
        desiredStepInterval = 0.0f;
        currentStepPulse = 0.0f;
        previousStepPulse = 0;
        return;
    }

    currentDirection = (speed_rps > 0 ? FORWARD : BACKWARD);
    desiredStepInterval = 5000.0f / (float)pulsePeriod_us / (float)MICROSTEPS / speed_rps_abs;
    currentStepPulse = desiredStepInterval;
    previousStepPulse = 0;
}