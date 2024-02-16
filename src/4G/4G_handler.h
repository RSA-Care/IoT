#define TINY_GSM_MODEM_SIM7600
#define SerialMon Serial
#define TINY_GSM_DEBUG SerialMon
#define TINY_GSM_USE_GPRS true
#define TINY_GSM_USE_WIFI false
#define GSM_AUTOBAUD_MIN 9600
#define GSM_AUTOBAUD_MAX 115200
#define DUMP_AT_COMMANDS true

#define TINY_GSM_TEST_GPRS true
#define TINY_GSM_TEST_TCP true
#define TINY_GSM_TEST_GPS true

#define RX_GSM 16
#define TX_GSM 17
#define defaultBaud 115200

#include <TinyGsmClient.h>
#include <HardwareSerial.h>
#include <StreamDebugger.h>

// Your GPRS credentials, if any
const char apn[] = "M2MINTERNET";
const char gprsUser[] = "";
const char gprsPass[] = "";

// Your WiFi connection credentials, if applicable
const char wifiSSID[] = "YourSSID";
const char wifiPass[] = "YourWiFiPass";

class SIM7600G
{
private:
  void debug();
  boolean _reset();

public:
  SIM7600G();
  boolean init(boolean reset = false);
  void manualCommand();
};
