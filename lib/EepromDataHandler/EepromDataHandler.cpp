#include "EepromDataHandler.h"

void EepromDataHandlerClass::setPids(float sP, float sI, float aP, float aD)
{
    pids[0] = sP;
    pids[1] = sI;
    pids[2] = aP;
    pids[3] = aD;
}

float* EepromDataHandlerClass::getPids()
{
    return pids;
}

void EepromDataHandlerClass::writeWifiDataToEeprom()
{
    int address = 0;
    clearEeprom(address,2*BLOCK_SIZE);
    for(int i=0; i<ssid.length();i++)
    {
        EEPROM.write(i+address,ssid[i]);
    }
    address+=BLOCK_SIZE;
    for(int i=0; i<password.length();i++)
    {
        EEPROM.write(i+address,password[i]);
    }
    if(EEPROM.commit())Serial.printf("ssid: %s and password: %s written to EEPROM succesfully\n",ssid.c_str(),password.c_str());
}

void EepromDataHandlerClass::writePidsToEeprom()
{
    int address = 2*BLOCK_SIZE;
    clearEeprom(address, 4*FLOAT_SIZE);
    for(int i=0; i<4; i++)
    {
        EEPROM.put(address+i*FLOAT_SIZE,pids[i]);
    }
    
    if(EEPROM.commit())Serial.printf("pids written to EEPROM succesfully\n");
}

void EepromDataHandlerClass::clearEeprom(int from, int to)
{
    int address = from;
    for(int i=from; i<to;i++)
    {
        EEPROM.write(i,0);
    }
}

void EepromDataHandlerClass::readWifiDataFromEeprom()
{
    ssid ="";
    password = "";
    int address = 0;
    for(int i=0; i<BLOCK_SIZE; i++)
    {
        ssid+=(char)EEPROM.read(i+address);
    }
    address+=BLOCK_SIZE;
    for(int i=0; i<BLOCK_SIZE; i++)
    {
        password+=(char)EEPROM.read(i+address);
    }

    Serial.printf("ssid: %s and password: %s read from EEPROM \n",ssid.c_str(),password.c_str());
}

void EepromDataHandlerClass::readPidsFromEeprom()
{
    int address = 2*BLOCK_SIZE;
    for(int i=0; i<4; i++)
    {
        EEPROM.get(address+i*FLOAT_SIZE,pids[i]);
    }
    
    Serial.printf("pids read from EEPROM succesfully\n");
}

void EepromDataHandlerClass::init()
{
    EEPROM.begin(2*BLOCK_SIZE+4*FLOAT_SIZE);
}
EepromDataHandlerClass EepromDataHandler;

