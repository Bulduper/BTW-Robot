#include <Arduino.h>
#include <ArduinoJson.h>
//#include <ReceivedDataPacket.h>

class ReceivedDataPacket;

namespace PacketParser
{
        ReceivedDataPacket stringToJsonData(String strData);
}