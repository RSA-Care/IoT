#include "4G_handler.h"

const char *broker = "broker.hivemq.com"; // MQTT Credential

HardwareSerial SerialAT(2);

#ifdef DUMP_AT_COMMANDS
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm _modem(debugger);
#else
TinyGsm _modem(SerialAT);
#endif

// TinyGsm _modem(SerialAT);
TinyGsmClient _client(_modem);

SIM7600G::SIM7600G()
{
  gpio_pad_select_gpio(PWRKEY);
  gpio_set_direction(PWRKEY, GPIO_MODE_OUTPUT);
  gpio_set_level(PWRKEY, 1);
}

boolean SIM7600G::_reset()
{
  return _modem.factoryDefault();
}

void SIM7600G::info()
{
  _modem.getGNSSMode();
  String imei = _modem.getIMEI();
  // String modemInfo = _modem.getModemInfo();
  boolean isNetworkConnected = _modem.isNetworkConnected();
  IPAddress ip;
  if (isNetworkConnected)
    ip = _modem.localIP();

  delay(2000);
  SerialMon.println(F("\n-------------------------------"));
  SerialMon.print(F("IMEI: "));
  SerialMon.println(imei);
  delay(1000);

  SerialMon.print(F("Modem info: "));
  _modem.getModemInfo();

  if (isNetworkConnected)
  {
    SerialMon.print(F("APN: "));
    SerialMon.println(apn);
    SerialMon.print(F("IP: "));
    SerialMon.print(ip[0], DEC);
    SerialMon.println('.');
  }

  SerialMon.println(F("-------------------------------"));

  delay(10000);
}

void SIM7600G::togglePWR()
{
  gpio_set_level(PWRKEY, 0); // Set the POWER KEY to LOW
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  gpio_set_level(PWRKEY, 1); // Set the POWER KEY back to HIGH
}

bool SIM7600G::init(boolean reset)
{
  if (reset)
  {
    SerialMon.println("Reseting the module...");
    _reset();
  }

  int counter = 0;
  SerialMon.print("Waiting for serial AT...");
  while (SerialAT.available() == 0 && counter < 5)
  {
    SerialMon.print(".");
    SerialAT.begin(defaultBaud, SERIAL_8N1, RX_GSM, TX_GSM, false);
    delay(3000);
    counter++;
  }

  if (SerialAT.available() == 0)
  {
    SerialMon.println("\nERROR: Failed to initialize GSM modem!");
    return false;
  }

  counter = 0;
  SerialMon.println("\nInitializing SIM7600G...");
  bool init = _modem.init();
  while (!_modem.restart() && counter < 5 && !init)
  {
    SerialMon.print("[ ");
    SerialMon.print(counter);
    SerialMon.println(" ] Re-initializing SIM7600G...");
    delay(3000);
    counter++;
  }
  // _modem.init();
  // _modem.restart();

  SerialMon.println("Preparing for SIM7600G power on.");
  delay(14000);

  SerialMon.print("Setting Baud Rate to ");
  SerialMon.println(defaultBaud);
  _modem.setBaud(defaultBaud);
  delay(3000);

  SerialMon.println("Enabling GPS...");
  bool gpsEnabled = _modem.enableGPS();
  if (gpsEnabled)
  {
    gps_enabled = gpsEnabled;
    SerialMon.println("GPS enabled");
  }

  info();

  connect();

  SerialMon.println(F("============================="));
  return true;
  // SerialAT.begin(defaultBaud, SERIAL_8N1, RX_GSM, TX_GSM, false);
}

bool SIM7600G::connect()
{
  SerialMon.println("Socket parameter:");
  _modem.sendAT("+CIPCCFG=?");
  delay(3000);

  SerialMon.println("Connecting to GPRS...");
  int counter = 0;
  while (!_modem.gprsConnect(apn) && counter < 5)
  {
    SerialMon.println("Reconnecting...");
    counter++;
    delay(10000);
  }
  // _modem.gprsConnect(apn);

  SerialMon.println();

  if (!_modem.waitForNetwork(10000L))
  { // You may need lengthen this in poor service areas
    SerialMon.println(F(" [fail]"));
    SerialMon.println(F("************************"));
    SerialMon.println(F(" Is your sim card locked?"));
    SerialMon.println(F(" Do you have a good signal?"));
    SerialMon.println(F(" Is antenna attached?"));
    SerialMon.println(F(" Does the SIM card work with your phone?"));
    SerialMon.println(F("************************"));
    delay(10000);
  }

  networkConnected = _modem.isNetworkConnected();
  return _modem.isGprsConnected();
}

bool SIM7600G::getNetworkStatus()
{
  return networkConnected;
}

String SIM7600G::getGPS()
{
  if (SerialAT.available() == 0)
  {
    SerialMon.println("Enabling Serial AT...");
    SerialAT.begin(defaultBaud, SERIAL_8N1, RX_GSM, TX_GSM, false);
    delay(500);
    SerialAT.available() ? SerialMon.println("Serial AT Enabled.") : SerialMon.println("Failed to enable serial AT.");
  }

  if (!gps_enabled)
  {
    _modem.enableGPS();
    _modem.getGNSSMode();
    gps_enabled = true;
    SerialMon.println("GPS Enabled.");
  }

  float lat, lon;

  _modem.getGPS(&lat, &lon);
  SerialMon.println(lat);
  SerialMon.println(lon);

  return _modem.getGPSraw();
}

void SIM7600G::debug()
{
  if (SerialAT.available())
    Serial.write(SerialAT.read()); // Arduino send the computer command to SIMCOM 7000

  if (Serial.available())
    SerialAT.write(Serial.read()); // Arduino send the SIMCOM 7000 feedback to computer
}