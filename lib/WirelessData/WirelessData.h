// WirelessData.h

#ifndef _WIRELESSDATA_h
#define _WIRELESSDATA_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <map>

//typedef void (*OnDataReceived)(char* data);
enum ConnectionStatus
{
	noConnection,
	connectedToWifi,
	apMode
};

class WirelessDataClass
{
protected:
	bool isUdpReady;
	int currentStatus=0;
	int connectionTimeout = 15;
	uint32_t mControllerIpAddress = 0xFFFFFFFF;
	IPAddress mLastRemoteIpAddress;
	String apSsid = "Robot", apPassword= "";
	WiFiUDP udp;
	const unsigned int mReceiverPort = 1234;
	const unsigned int mMyPort = 4321;
	char incomingPacket[255];
	std::map<byte, String> dataToSend;
	void (*OnDataReceived)(char* data);

public:
	void init(String, String);
	bool connectToWiFi(String, String);
	bool beginAP();
	void startUdp(void func(char *));
	void stopUdp();
	void send(String msg, uint32_t ipAddress);
	void send(String msg);
	IPAddress convertIPToInt32(uint8_t,uint8_t,uint8_t,uint8_t);
	IPAddress convertIPToInt32(String);
	void setTargetIpAddress(uint32_t);
	void setLastRemoteIpAddress() { mControllerIpAddress = mLastRemoteIpAddress.v4(); }
	void listen();
	void chainSend(byte, String);
	void loop();
};

extern WirelessDataClass WirelessData;

#endif
