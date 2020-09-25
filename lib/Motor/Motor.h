#ifndef _MOTOR_H
#define _MOTOR_H

#include <Arduino.h>

#define MICROSTEPS 16


enum Direction
{
    FORWARD,
    BACKWARD
};

class Motor
{
    private:
    byte stepPin, dirPin, enPin;
    uint16_t pulsePeriod_us;
    uint32_t pulseCounter = 0;
    uint32_t currentPulseInterval=0;
    bool currentDirection;
    bool side;

    public:
    void init(byte,byte,byte,bool,uint16_t);
    void enable();
    void disable();
    void run();
    void setRPS(float);
};
#endif