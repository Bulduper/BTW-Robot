#include <Arduino.h>
#include <MotorController.h>
#include <BalanceRegulator.h>
#include <WirelessData.h>
#include <PacketCreator.h>
#include <PacketParser.h>
#include <EepromDataHandler.h>

#define stepR 14
#define dirR 16
#define enR 5
#define stepL 13
#define dirL 12
#define enL 4
#define SDA 2
#define SCL 0

BalanceRegulator Regulator;

void loop10Hz();
void sendFeedbackPacket();
void sendTelemetryPacket();
void setTimerInterrupt();
void onDataReceived(char *data);
void setSteeringData(ReceivedDataPacket data);
void setPidValues(ReceivedDataPacket data);
void updateDefaultWifiCredentials(ReceivedDataPacket data);
void setIncomingDataPeriod(ReceivedDataPacket data);
void ICACHE_RAM_ATTR ISR();

bool remoteControllerFound = false;
bool sendTelemetry = true;
String telemetryReceiverIPAddress = "192.168.1.18";
int udpIdealPeriod, udpRealPeriod;

void setup()
{
  //new idea: try to connect to the default wifi (saved in memory), if this will not succeed - set the AP
  //default wifi will be modifable by a json command + reset handling

  Serial.begin(115200);
  motors.begin(stepL, stepR, dirL, dirR, enL, enR, false);

  Wire.begin(SDA, SCL);
  delay(200);
  MPU6050_setup();
  //wireless init part
  EepromDataHandler.init();
  EepromDataHandler.readWifiDataFromEeprom();
  EepromDataHandler.readPidsFromEeprom();
  Regulator.setSpeedPidP(EepromDataHandler.getPids()[0]);
  Regulator.setSpeedPidI(EepromDataHandler.getPids()[1]);
  Regulator.setAnglePidP(EepromDataHandler.getPids()[2]);
  Regulator.setAnglePidD(EepromDataHandler.getPids()[3]);

  if(!WirelessData.connectToWiFi(EepromDataHandler.getSsid(),EepromDataHandler.getPassword()))
  {
    WirelessData.beginAP();
  }

  WirelessData.startUdp(onDataReceived);

  delay(500);
  MPU6050_calibrate();
  setTimerInterrupt();

  motors.readyRoutine();
}

void loop()
{
  WirelessData.loop();
  Regulator.regulateLoop();
  loop10Hz();
}

void setTimerInterrupt()
{
  noInterrupts();
  timer1_isr_init();
  timer1_attachInterrupt(ISR);
  timer1_enable(TIM_DIV1, TIM_EDGE, TIM_LOOP);
  timer1_write(uint32_t(80 * PULSE_PERIOD_us));
  interrupts();
}

void ICACHE_RAM_ATTR ISR()
{
  motors.run();
}

void loop10Hz()
{
  static uint32_t lastT;
  if (micros() - lastT >= 100000)
  {
    if(remoteControllerFound)sendFeedbackPacket();
    if(sendTelemetry)sendTelemetryPacket();
    lastT = micros();
  }
}

void sendFeedbackPacket()
{
  WirelessData.send(PacketCreator::feedbackDataToJsonString(
      Regulator.getLeftVelocity(),
      Regulator.getRightVelocity(),
      Regulator.getCurrentAngle(),
      Regulator.getSpeedPidP(),
      Regulator.getSpeedPidI(),
      Regulator.getAnglePidP(),
      Regulator.getAnglePidD(),
      udpRealPeriod-udpIdealPeriod));
}

void sendTelemetryPacket()
{
  WirelessData.send(PacketCreator::createTelemetryPacket(
      Regulator.getCurrentAngle(),
      Regulator.getRightVelocity(),
      motors.getRPulseInterval()),
    convertIPToInt32(192,168,1,18));
}

void setSteeringData(ReceivedDataPacket data)
{
  Regulator.setRelativeExpectedSpeed(data.getValueInt("throttle"));
  Regulator.setRelativeTurn(data.getValueInt("yaw"));
}

void setPidValues(ReceivedDataPacket data)
{
  Regulator.setSpeedPidP(data.getValueFloat("speedPidP"));
  Regulator.setSpeedPidI(data.getValueFloat("speedPidI"));
  Regulator.setAnglePidP(data.getValueFloat("anglePidP"));
  Regulator.setAnglePidD(data.getValueFloat("anglePidD"));
}

void updateDefaultWifiCredentials(ReceivedDataPacket data)
{
  String ssid, password;
  ssid = data.getValueString("ssid");
  password = data.getValueString("password");
  Serial.printf("New network data - ssid: %s, password: %s\n",ssid.c_str(),password.c_str());
  EepromDataHandler.setSsid(ssid);
  EepromDataHandler.setPassword(password);
  noInterrupts();
  EepromDataHandler.writeWifiDataToEeprom();
  interrupts();
}

void setIncomingDataFPeriod(ReceivedDataPacket data)
{
  udpIdealPeriod = data.getValueInt("interval");
}

void onDataReceived(char *data)
{
  static unsigned int lT;
  //Serial.printf("%u\t%u\tdataRec: %s\n", millis(), millis() - lT, data);
  udpRealPeriod = millis()-lT;
  lT = millis();

  ReceivedDataPacket dataPacket = PacketParser::stringToJsonData(String(data));
  String receivedDataType = dataPacket.getPacketType();

  if (receivedDataType == "welcome")
  {
    WirelessData.setLastRemoteIpAddress();
    remoteControllerFound = true;
  }
  else if (receivedDataType == "steering")
  {
    setSteeringData(dataPacket);
  }
  else if (receivedDataType == "pids")
  {
    setPidValues(dataPacket);
  }
  else if (receivedDataType == "setNetwork")
  {
    updateDefaultWifiCredentials(dataPacket);
    WirelessData.send(PacketCreator::infoMessageToJsonString("Default Network Updated"));
  }
    else if (receivedDataType == "saveParams")
  {
    EepromDataHandler.setPids(Regulator.getSpeedPidP(),Regulator.getSpeedPidI(),Regulator.getAnglePidP(),Regulator.getAnglePidD());
    noInterrupts();
    EepromDataHandler.writePidsToEeprom();
    interrupts();
    WirelessData.send(PacketCreator::infoMessageToJsonString("PIDs Updated"));
  }
}