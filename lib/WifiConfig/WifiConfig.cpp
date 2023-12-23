#include <WifiConfig.h>

const char *mqtt_server = "0ee29c0de112499cb84bcc2b3f5667dd.s2.eu.hivemq.cloud";
const char *mqtt_username = "Duy2504";
const char *mqtt_password = "Duy1002452";
const int mqtt_port = 8883;
const char *root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";
bool checkSmartConfig = false;
WifiConfig::WifiConfig()
{
}

WifiConfig::~WifiConfig()
{
}
bool in_smartconfig = false;
void WifiConfig::enter_smartconfig()
{
    if (in_smartconfig == false)
    {
        in_smartconfig = true;
        WiFi.disconnect();
        WiFi.mode(WIFI_STA);
        WiFi.beginSmartConfig();
        Serial.println("Enter smartconfig");
        deviceService.ledWifi_Off();
        deviceService.ledSM_On();
    }
}

void WifiConfig::exit_smart()
{
    in_smartconfig = false;
    String ssid = WiFi.SSID();
    String pass = WiFi.psk();
    Serial.println(ssid);
    Serial.print("Pass: ");
    Serial.println(pass);
    cEEPROM.write(ssid, 0, 19);
    cEEPROM.write(pass, 20, 39);
    WiFi.stopSmartConfig();
    Serial.println("Connected, Exit smartconfig");
    deviceService.ledSM_Off();
    delay(500);
    wifiConfig.ConnectWifi(ssid, pass);
}

void WifiConfig::ConnectWifi(String ssid, String pass)
{
    WiFi.reconnect();
    Serial.print("Connecting WiFi: ");
    Serial.println(ssid);
    Serial.print("Pass: ");
    Serial.println(pass);
    delay(5);
    WiFi.begin(ssid, pass);
    Serial.print("Connecting ...");
    deviceService.ReadyLedToogle();
    while (WiFi.status() != WL_CONNECTED)
    {
        deviceService.ledWifi_Off();
        delay(500);
        Serial.print(".");
        if (deviceService.longPress())
        {
            enter_smartconfig();
        }
        if (WiFi.status() == WL_CONNECTED && in_smartconfig && WiFi.smartConfigDone())
        {
            exit_smart();
        }
    }
    delay(500);
    Serial.println("");
    Serial.println("WiFi Connection success!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    deviceService.ledWifi_On();
    deviceService.ReadyLedOn();
}

void WifiConfig::setupWifi()
{
    String ssid = cEEPROM.getWiFissid();
    String pass = cEEPROM.getWiFipassword();
    ConnectWifi(ssid, pass);
}

WifiConfig wifiConfig;
