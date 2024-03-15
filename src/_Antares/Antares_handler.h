#include <AntaresESP32MQTT.h>

#define ACCESSKEY "5857af4ec372ee95:d546da27532c4a59"
#define PROJECT "tracker"
#define DEVICE "Testing"

// Use for testing only
#define _SSID "bahagia permai"
#define _PASS "imut2023"

class Antares
{
private:
  struct _dat
  {
    float lat = 0;
    float lon = 0;
    float temp = 0;
    float hum = 0;
  };

  float lat;
  float lon;
  float temp;
  float hum;

public:
  Antares();
  bool init();
  void addData(_dat data);
  bool publish(boolean test);
};
