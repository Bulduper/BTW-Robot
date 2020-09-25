#include "PacketCreator.h"


String PacketCreator::feedbackDataToJsonString(float leftVelocity, float rightVelocity, float angle, float speedPidP, float speedPidI, float anglePidP, float anglePidD, int delay)
{
    const int capacity = JSON_OBJECT_SIZE(10);
    StaticJsonDocument<capacity> doc;

    doc["type"].set("feedback");
    doc["data"]["left_velocity"].set(leftVelocity);
    doc["data"]["right_velocity"].set(rightVelocity);
    doc["data"]["angle"].set(angle);
    doc["data"]["speedPidP"].set(speedPidP);
    doc["data"]["speedPidI"].set(speedPidI);
    doc["data"]["anglePidP"].set(anglePidP);
    doc["data"]["anglePidD"].set(anglePidD);
    doc["data"]["delay"].set(delay);


    String jsonString;
    serializeJson(doc,jsonString);
    return jsonString;
}

String PacketCreator::infoMessageToJsonString(String message)
{
    const int capacity = JSON_OBJECT_SIZE(10);
    StaticJsonDocument<capacity> doc;
    JsonObject data;
    doc["type"].set("info");
    doc["data"]["message"].set(message);
    
    String jsonString;
    serializeJson(doc,jsonString);
    return jsonString;

}

String PacketCreator::createTelemetryPacket(float par1,float par2,float par3,float par4,float par5,float par6)
{
    return String(par1) + "," + String(par2) + ","+ String(par3) + ","+ String(par4) + ","+ String(par5) + ","+ String(par6) + "\n";
}

