#ifndef _EEPROM_DATA_HANDLER_H
#define _EEPROM_DATA_HANDLER_H
#define BLOCK_SIZE 64
#define FLOAT_SIZE 4
#include <Arduino.h>
#include <EEPROM.h>

class EepromDataHandlerClass
{
    private:
    String ssid;
    String password;
    float pids[4];

    public:
    void setSsid(String s){ssid = s;};
    void setPassword(String p){password = p;}
    void setPids(float sP, float sI, float aP, float aD);
    float* getPids();
    String getSsid(){return ssid;}
    String getPassword(){return password;}

    void writeWifiDataToEeprom();
    void readWifiDataFromEeprom();
    void writePidsToEeprom();
    void readPidsFromEeprom();
    void clearEeprom(int from, int to);
    void init();
};

extern EepromDataHandlerClass EepromDataHandler;

#endif