#ifndef _CONFIG_WIFI_H_
#define _CONFIG_WIFI_H_

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <CustomEEPROM.h>
#include <DeviceService.h>
class WifiConfig
{
private:
    bool smConfigMode = false;
    String ssid;
    String pass;
    WiFiClient espClient;
    PubSubClient client;
    DeviceService device;

public:
    WifiConfig();
    ~WifiConfig();
    void ConnectWifi(String ssid, String pass);
    void setupWifi();
    void enter_smartconfig();
    void exit_smart();
};
extern bool in_smartconfig;
extern const char *root_ca;
extern const char *mqtt_server;
extern const char *mqtt_username;
extern const char *mqtt_password;
extern const int mqtt_port;
extern WifiConfig wifiConfig;

#endif /* _CONFIG_WIFI_H_ */