//#include <Arduino.h>
#include <ArduinoJson.h>

class ReceivedDataPacket
{
    private:
    //const size_t capacity = 300;
    String packetType;
    StaticJsonDocument<300> jsonDoc;
    JsonObject data;

    public:
    ReceivedDataPacket();
    StaticJsonDocument<300>& getJson(){return jsonDoc;}
    void setPacketType();
    void setData();
    String getPacketType() {return packetType;}
    String getValueString(String);
    float getValueFloat(String);
    int getValueInt(String);

};