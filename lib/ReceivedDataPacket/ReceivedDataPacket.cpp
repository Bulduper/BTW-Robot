#include "ReceivedDataPacket.h"

ReceivedDataPacket::ReceivedDataPacket()
{
}
void ReceivedDataPacket::setPacketType()
{
    String pT = getJson().getMember("type");
    packetType = pT;
}
void ReceivedDataPacket::setData()
{
    data = getJson().getMember("data");
}

String ReceivedDataPacket::getValueString(String key)
{
    return data.getMember(key);
}
float ReceivedDataPacket::getValueFloat(String key)
{
    return data[key];
}

int ReceivedDataPacket::getValueInt(String key)
{
    return data[key];
}