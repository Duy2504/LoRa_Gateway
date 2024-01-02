#include <Arduino.h>
#include <CustomEEPROM.h>
#include "Ticker.h"
#ifndef _DEVICESERVICE_H_
#define _DEVICESERVICE_H_
class DeviceService
{
private:
    static const int readyLed = 2;
    static const int ledWifi = 12;
    static const int ledSM   = 14;
    static const int buttonWifi = 13;

public:
    void setup();
    void ReadyLedOn();
    void ReadyLedOff();
    static void tick();
    void ReadyLedToogle();
    bool longPress();
    void ledWifi_On();
    void ledWifi_Off();
    void ledWifi_Toggle();
    void ledSM_On();
    void ledSM_Off();
};
extern DeviceService deviceService;
#endif /* _DEVICESERVICE_H_*/