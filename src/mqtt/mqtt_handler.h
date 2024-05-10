#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>

class MQTTHandler
{
private:
  String clientId;
  bool checkWIFI();
  String stateCheck();
  void reconnect();
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
  // bool publish();
  bool publish(float longitude, float latitude, float temperature, float humidity);
  // void callback(char *topic, byte *payload, unsigned int length);
};