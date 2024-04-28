#define TINY_GSM_MODEM_SIM7600
#define SerialMon Serial
#define TINY_GSM_DEBUG SerialMon
#define TINY_GSM_USE_GPRS true
#define TINY_GSM_USE_WIFI false
#define GSM_AUTOBAUD_MIN 9600
#define GSM_AUTOBAUD_MAX 115200
#define DUMP_AT_COMMANDS

#define TINY_GSM_TEST_GPRS true
#define TINY_GSM_TEST_TCP true
#define TINY_GSM_TEST_GPS true

#define RX_GSM 16
#define TX_GSM 17
#define defaultBaud 115200

#define PWRKEY GPIO_NUM_19

#include <TinyGSM.h>
#include <TinyGsmClient.h>
#include <HardwareSerial.h>
#include <StreamDebugger.h>

// Your GPRS credentials, if any
const char apn[] = "internet";
const char gprsUser[] = "";
const char gprsPass[] = "";

class SIM7600G
{
private:
  void debug();
  void info();
  bool serialATConnect();
  bool _connect2g();
  boolean _reset();
  boolean networkConnected = false;
  boolean gps_enabled = false;
  boolean startUp = false;
  const char *modemInfo;

public:
  SIM7600G();
  void togglePWR();
  bool init(boolean reset = false);
  bool connect(boolean connet_2g = false);
  String getGPS();
  String getInfo();
  bool getNetworkStatus();
};
