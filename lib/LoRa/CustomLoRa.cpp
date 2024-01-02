#include <CustomLoRa.h>

void CustomLoRa::setup_Lora()
{
  SPI.begin(PIN_LORA_SCK, PIN_LORA_MISO, PIN_LORA_MOSI);
  LoRa.setPins(PIN_LORA_CS, PIN_LORA_RST, PIN_LORA_DIO0);
  while (!LoRa.begin(LORA_FREQUENCY))
  {
    Serial.println("Starting LoRa failed!");
    delay(1000);
  }
  LoRa.setSpreadingFactor(LORA_SF);
  LoRa.setCodingRate4(LORA_CR);
  LoRa.setSignalBandwidth(LORA_BW);
  LoRa.setPreambleLength(LORA_PREAMBLE_LENGTH);
  LoRa.enableCrc();
  Serial.println("LoRa started OK!");
  //  LoRa.setSPIFrequency (20000000);
  //  LoRa.setTxPower (18);
  // put the radio into receive mode
  LoRa.receive();
}

bool checkData;
int chipId;
float temp, humi, light, soil, SNR, Rssi;
void CustomLoRa::tach_String(String data)
{
  // Tìm vị trí của các giá trị
  int idIndex = data.indexOf("ID: ");
  int tempIndex = data.indexOf("Temp: ");
  int humiIndex = data.indexOf("Humi: ");
  int lightIndex = data.indexOf("Light: ");
  int soilIndex = data.indexOf("Soil: ");
  int pinIndex = data.indexOf("Pin: ");

  // Tách chuỗi để lấy các giá trị
  String chipIdS = data.substring(idIndex + 4, tempIndex - 1);
  String tempS = data.substring(tempIndex + 6, humiIndex - 1);
  String humiS = data.substring(humiIndex + 6, lightIndex - 1);
  String lightS = data.substring(lightIndex + 7, soilIndex - 1);
  String soilS = data.substring(soilIndex + 6, pinIndex - 1);

  chipId = chipIdS.toInt();
  temp = tempS.toFloat();
  humi = humiS.toFloat();
  light = lightS.toFloat();
  soil = soilS.toFloat();
}
bool CustomLoRa::checkId(String data, String targetID)
{
  // Tìm vị trí của ID trong dữ liệu
  int idIndex = data.indexOf("ID: ");
  if (idIndex != -1)
  {
    // Lấy ID từ dữ liệu
    String receivedID = data.substring(idIndex + 4, idIndex + 11);
    // So sánh với giá trị mục tiêu
    return (receivedID == targetID);
  }
  return false; // Không tìm thấy ID trong dữ liệu
}

String CustomLoRa::receiveLoRaMessage()
{
  int packetSize = LoRa.parsePacket();
  if (packetSize == 0)
    return "";
  String incoming = "";
  while (LoRa.available())
  {
    incoming += (char)LoRa.read();
  }
  // incoming = "RSSI:" + String(LoRa.packetRssi()) +
  //            " | SNR:" + String(LoRa.packetSnr()) +
  //            " | " + incoming;
  Rssi = LoRa.packetRssi();
  SNR = LoRa.packetSnr();
  incoming = incoming;
  return incoming;
}

int customIdMaster = 6;
void CustomLoRa::sendMessage(String message)
{
  String control = "6," + message;
  Serial.print("Send control: ");
  LoRa.beginPacket();
  LoRa.print(control);
  Serial.println(control);
  LoRa.endPacket(true);
}

CustomLoRa customLoRa;