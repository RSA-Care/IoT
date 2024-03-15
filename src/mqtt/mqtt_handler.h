#include <WiFi.h>

#define _SSID "bahagia permai"
#define _PASS "imut2023"

class mqtt_handler
{
private:
  bool checkWIFI();
  boolean useWIFI = false;

public:
  mqtt_handler();
  bool setupWIFI();
  bool publish(String gps, float temperature, float humidity);
};