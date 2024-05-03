#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>

// #define _SSID "Ahda's"
// #define _PASS "@hotspot.Personal"

// #define _SSID "bahagia permai"
// #define _PASS "imut2023"

class MQTTHandler
{
private:
  bool checkWIFI();
  const char *_SSID;
  const char *_PASS;
  boolean useWIFI = false;
  String errorStatus;
  // void println(String text);

public:
  MQTTHandler(const char *SSID, const char *PASS);
  String getStatus();
  bool setupWIFI();
  bool MQTTconnect(boolean test = false);
  bool publish();
  bool publish(String gps, float temperature, float humidity);
  // void callback(char *topic, byte *payload, unsigned int length);
};