#include <Arduino.h>

class SteeringData
{
    private:
    byte mThrottle, mYaw;
    float mSpeedPidP, mSpeedPidI, mAnglePidP, mAnglePidD;

    public:
    void setThrottle(byte throttle){mThrottle = throttle;}
    void setYaw(byte yaw){mYaw = yaw;}
    void setSpeedPidP(float p){mSpeedPidP = p;}
    void setSpeedPidI(float i){mSpeedPidI = i;}
    void setAnglePidP(float p){mAnglePidP = p;}
    void setAnglePidD(float d){mAnglePidD = d;}

    byte getThrottle(){return mThrottle;}
    byte getYaw(){return mYaw;}
    float getSpeedPidP(){return mSpeedPidP;}
    float getSpeedPidI(){return mSpeedPidI;}
    float getAnglePidP(){return mAnglePidP;}
    float getAnglePidD(){return mAnglePidD;}
};