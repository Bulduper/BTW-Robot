#include "PacketParser.h"
#include <ReceivedDataPacket.h>

ReceivedDataPacket PacketParser::stringToJsonData(String strData)
{
    ReceivedDataPacket dataPacket;
    const char* typeOfPacket;
    int throttle, yaw;
    float sP,sI,aP,aD;
    deserializeJson(dataPacket.getJson(),strData);
    dataPacket.setData();
    dataPacket.setPacketType();

    return dataPacket;
}