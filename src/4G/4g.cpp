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
TinyGsmClient client(_modem);

SIM7600G::SIM7600G()
{
}

boolean SIM7600G::_reset()
{
  return _modem.factoryDefault();
}

boolean SIM7600G::init(boolean reset)
{
  if (reset)
  {
    SerialMon.println("Reseting the module...");
    _reset();
  }

  int counter = 0;
  while (SerialAT.available() == 0 && counter < 5)
  {
    SerialMon.println("Waiting for serial AT...");
    SerialAT.begin(defaultBaud, SERIAL_8N1, RX_GSM, TX_GSM, false);
    SerialMon.println(SerialAT.available());
    delay(3000);
    counter++;
  }

  if (SerialAT.available() == 0)
    return false;

  SerialMon.print("Setting Baud Rate to ");
  SerialMon.println(defaultBaud);
  _modem.setBaud(defaultBaud);
  delay(3000);

  counter = 0;
  SerialMon.println("Initializing SIM7600G...");
  while (!_modem.init() && counter < 5)
  {
    SerialMon.print("[ ");
    SerialMon.print(counter);
    SerialMon.println(" ] Initializing SIM7600G...");
    delay(3000);
    counter++;
  }
  // _modem.init();
  // _modem.restart();

  return true;
  // SerialAT.begin(defaultBaud, SERIAL_8N1, RX_GSM, TX_GSM, false);
}

boolean SIM7600G::connect()
{
  SerialMon.print("Sim Card Status : ");
  SerialMon.println(_modem.getSimStatus());
  delay(3000);

  String modemInfo = _modem.getModemInfo();
  SerialMon.print("Modem Info : ");
  SerialMon.println(modemInfo);
  delay(3000);

  SerialMon.println("Connecting to GPRS...");
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
  _modem.gprsConnect(apn);

  return _modem.isGprsConnected();
}

String SIM7600G::getGPS()
{
  if (SerialAT.available() == 0)
  {
    SerialAT.begin(defaultBaud, SERIAL_8N1, RX_GSM, TX_GSM, false);
    delay(500);
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