//
//
//

#include "WirelessData.h"

bool WirelessDataClass::connectToWiFi(String ssid, String password)
{
	WiFi.begin(ssid, password);
	byte timeoutCheck = 0;
	while (WiFi.status() != WL_CONNECTED)
	{
		yield();
		timeoutCheck++;
		delay(500);
		Serial.print(".");
		if (timeoutCheck > connectionTimeout * 2)
		{
			Serial.printf("\nFailed to connect to: %s\n", ssid.c_str());
			currentStatus = ConnectionStatus::noConnection;
			return false;
		}
	}
	Serial.printf("\nConnected to: %s\n", ssid.c_str());
	currentStatus = ConnectionStatus::connectedToWifi;
	return true;
}


bool WirelessDataClass::beginAP()
{
	//WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	IPAddress localIp(192, 168, 1, 1);
	IPAddress gateway(192, 168, 1, 1);
	IPAddress subnet(255, 255, 255, 0);
	//WiFi.softAPConfig(localIp, gateway, subnet);
	if (WiFi.softAP(apSsid, apPassword))
	{
		Serial.printf("Access Point set: ssid=%s, password=%s, server ip = %s", apSsid.c_str(), apPassword.c_str(), WiFi.softAPIP().toString().c_str());
		currentStatus = ConnectionStatus::apMode;
		return true;
	}
	else
	{
		Serial.printf("Access Point error!");
		currentStatus=ConnectionStatus::noConnection;
		return false;
	}
}

void WirelessDataClass::startUdp(void func(char *data))
{
	OnDataReceived = func;
	if (!currentStatus)
		return;
	udp.begin(mMyPort);
	Serial.printf("Now set at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), mMyPort);
	isUdpReady = true;
}

void WirelessDataClass::stopUdp()
{
	udp.stop();
	isUdpReady = false;
}

void WirelessDataClass::send(String msg, uint32_t ipAddress)
{
	if (!isUdpReady)
		return;
	udp.beginPacket(IPAddress(ipAddress), mReceiverPort);
	//Serial.printf("dataSent: %s\n",msg.c_str());
	udp.write(msg.c_str());
	udp.endPacket();
}

void WirelessDataClass::send(String msg)
{
	send(msg, mControllerIpAddress);
}

IPAddress WirelessDataClass::convertIPToInt32(uint8_t a,uint8_t b ,uint8_t c ,uint8_t d )
{
	return IPAddress(a,b,c,d);
}

IPAddress WirelessDataClass::convertIPToInt32(String ip_string)
{
	return IPAddress();
}


void WirelessDataClass::chainSend(byte key, String message)
{
	std::pair<std::map<byte, String>::iterator, bool> newPair = dataToSend.insert(std::pair<byte, String>(key, message));
	if (!newPair.second)
		newPair.first->second = message;
}

void WirelessDataClass::listen()
{
	if (!isUdpReady)
		return;
	int packetSize = udp.parsePacket();
	if (packetSize)
	{
		int length = udp.read(incomingPacket, 255);
		if (length)
			incomingPacket[length] = 0;
		//Serial.printf("Packet received from IP: %s\n", udp.remoteIP().toString().c_str());
		mLastRemoteIpAddress = udp.remoteIP();
		OnDataReceived(incomingPacket);
	}
}

void WirelessDataClass::loop()
{
	listen();
}

WirelessDataClass WirelessData;
