#include "PacketParser.h"
#include <ReceivedDataPacket.h>

ReceivedDataPacket PacketParser::stringToJsonData(String strData)
{

    //const int capacity = JSON_OBJECT_SIZE(12);
    //StaticJsonDocument<capacity> doc;
    ReceivedDataPacket dataPacket;
    const char* typeOfPacket;
    int throttle, yaw;
    float sP,sI,aP,aD;
    deserializeJson(dataPacket.getJson(),strData);
    dataPacket.setData();
    dataPacket.setPacketType();

    //Serial.printf("%f\t%f\t%f\t%f\n",sP,sI,aP,aD);
    //dataPacket.setThrottle(byte(throttle));
    //dataPacket.setYaw(byte(yaw));
    //dataPacket.setSpeedPidP(sP);       
    //dataPacket.setSpeedPidI(sI);
    //dataPacket.setAnglePidP(aP);
    //dataPacket.setAnglePidD(aD);
        
    return dataPacket;
}