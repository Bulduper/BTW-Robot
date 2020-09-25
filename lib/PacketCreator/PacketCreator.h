#include <ArduinoJson.h>
#include <ReceivedDataPacket.h>
namespace PacketCreator
{
    String feedbackDataToJsonString(float leftVelocity, float rightVelocity, float angle, float speedPidP, float speedPidI, float anglePidP, float anglePidD, int delay);
    String infoMessageToJsonString(String message);
    String createTelemetryPacket(float=0.0f,float=0.0f,float=0.0f,float=0.0f,float=0.0f,float=0.0f);
};