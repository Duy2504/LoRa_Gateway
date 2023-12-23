#include <DeviceService.h>

Ticker ticker;
void DeviceService::setup()
{
  pinMode(readyLed, OUTPUT);
  digitalWrite(readyLed, HIGH);
  pinMode(buttonWifi, INPUT);
  pinMode(ledWifi, OUTPUT);
  digitalWrite(ledWifi, LOW);
  pinMode(ledSM, OUTPUT);
  digitalWrite(ledSM, LOW);
  Serial.begin(115200);
  EEPROM.begin(2048);
  delay(5);
}
void DeviceService::ledWifi_On()
{
  digitalWrite(ledWifi, HIGH);
}
void DeviceService::ledWifi_Off()
{
  digitalWrite(ledWifi, LOW);
}

void DeviceService::ledSM_On()
{
  digitalWrite(ledSM, HIGH);
}
void DeviceService::ledSM_Off()
{
  digitalWrite(ledSM, LOW);
}

void DeviceService::ReadyLedOn()
{
  digitalWrite(readyLed, HIGH);
  ticker.detach();
}
void DeviceService::ReadyLedOff()
{
  digitalWrite(readyLed, LOW);
  ticker.detach();
}
void DeviceService::tick()
{
  int state = digitalRead(readyLed);
  digitalWrite(readyLed, !state);
}
void DeviceService::ReadyLedToogle()
{
  ticker.attach(1, tick);
}
bool DeviceService::longPress()
{
  static int lastPress = 0;
  if (millis() - lastPress > 3000 && digitalRead(13) == 0)
  {
    return true;
  }
  else if (digitalRead(13) == 1)
  {
    lastPress = millis();
  }
  return false;
}
DeviceService deviceService;