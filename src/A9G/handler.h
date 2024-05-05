#include <HardwareSerial.h>

#define RX_A9G 16
#define TX_A9G 17

#define defaultBaud 115200

HardwareSerial a9gSerial(2);

class a9g
{
private:
public:
  a9g();
  bool begin();
  String getGPS();
};