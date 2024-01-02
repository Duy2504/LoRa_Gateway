#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <LoRa.h>

// Thư viện Custom
#include <DeviceService.h>
#include <CustomEEPROM.h>
#include <WifiConfig.h>
#include <CustomLoRa.h>

// WiFiClient espClient;
WiFiClientSecure espClient;
PubSubClient client(espClient);

//========== TOPIC Control ==========//
#define sub1 "light_state"
#define sub2 "pump_state"
#define sub3 "rem_state"

void Pub();
void readSensor();
void MQTT()
{
  while (!client.connected())
  {
    Serial.println("Attempting MQTT connection...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password))
    {
      Serial.println("MQTT connected!");
      client.subscribe(sub1);
      client.subscribe(sub2);
      client.subscribe(sub3);
      deviceService.ledWifi_On();
    }
    else
    {
      Serial.println(" try again in 2 seconds");
      Serial.print("failed, rc=");
      Serial.println(client.state());
      delay(2000);
      deviceService.ledWifi_Off();
    }
  }
  client.loop();
}
void callback(char *topic, byte *payload, unsigned int length)
{
  String subData1 = "";
  String subData2 = "";
  String subData3 = "";

  if (strstr(topic, sub1))
  {
    for (int i = 0; i < length; i++)
    {
      subData1 += (char)payload[i];
    }
    // Serial.println(subData1);
    delay(1000);
    customLoRa.sendMessage(subData1);
  }
  else if (strstr(topic, sub2))
  {
    for (int i = 0; i < length; i++)
    {
      subData2 += (char)payload[i];
    }
    // Serial.println(subData2);
    delay(2000);
    customLoRa.sendMessage(subData2);
  }
  else if (strstr(topic, sub3))
  {
    for (int i = 0; i < length; i++)
    {
      subData3 += (char)payload[i];
    }
    // Serial.println(subData3);
    customLoRa.sendMessage(subData3);
  }
}
void setup()
{
  Serial.begin(115200);
  deviceService.setup();
  wifiConfig.setupWifi();
  customLoRa.setup_Lora();
  espClient.setCACert(root_ca);
  espClient.setInsecure();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}
void loop()
{
  if (deviceService.longPress())
  {
    wifiConfig.enter_smartconfig();
  }
  if (WiFi.status() == WL_CONNECTED && in_smartconfig && WiFi.smartConfigDone())
  {
    wifiConfig.exit_smart();
  }
  if(WiFi.status() == WL_CONNECTED)
  {
    MQTT();
    readSensor();    
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    deviceService.ledWifi_Off();
  }
}

/* Funtion */
void Pub()
{
  DynamicJsonDocument jsonDoc(128);
  jsonDoc["deviceId"] = chipId;
  jsonDoc["temperature"] = temp;
  jsonDoc["humidity"] = humi;
  jsonDoc["light"] = light;
  jsonDoc["soil"] = soil;
  jsonDoc["Rssi"] = Rssi;
  jsonDoc["SNR"] = SNR;
  String jsonString;
  serializeJson(jsonDoc, jsonString);
  client.publish("sensor", jsonString.c_str());
  Serial.println(jsonString);
}
void readSensor()
{
  String loraMessage = customLoRa.receiveLoRaMessage();
  if (customLoRa.checkId(loraMessage, "4402492") || customLoRa.checkId(loraMessage, "1432308"))
  {
    Serial.print(loraMessage);
    Serial.print("RSSI: ");
    Serial.println(Rssi);
    customLoRa.tach_String(loraMessage);
    checkData = true;
  }
  else
  {
    checkData = false;
  }
  if (checkData)
  {
    Pub();
    checkData = false;
  }
}


