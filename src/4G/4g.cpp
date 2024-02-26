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

SIM7600G::SIM7600G() {}

boolean SIM7600G::_reset()
{
  return _modem.factoryDefault();
}

boolean SIM7600G::init(boolean reset)
{
  SerialAT.begin(defaultBaud, SERIAL_8N1, RX_GSM, TX_GSM, false);

  if (_modem.isGprsConnected())
  {
    if (_modem.enableGPS())
    {
      SerialMon.println("GPS Enabled.");
    }

    return _modem.isGprsConnected();
  }

  delay(6000);

  SerialMon.print("Setting Baud Rate to ");
  SerialMon.println(defaultBaud);
  _modem.setBaud(defaultBaud);
  // TinyGsmAutoBaud(SerialAT, GSM_AUTOBAUD_MIN, GSM_AUTOBAUD_MAX); // use for testing

  if (reset)
  {
    SerialMon.println("Reseting the module...");
    _reset();
  }

  delay(6000);

  SerialMon.println("SerialAT Ready!");

  SerialMon.println("Initializing SIM7600G...");
  _modem.init();
  delay(3000);

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

  if (_modem.waitResponse(10000L) != 1)
  {
    Serial.println("GPS FAIL");
    // Serial.println("SGPIO=0,4,1,1 false ");
  }
  if (_modem.enableGPS())
  {
    SerialMon.println("[ + ] GPS Enabled.");
  }
  else
  {
    SerialMon.println("[ - ] GPS Disabled.");
  }

  // SerialMon.println("Setting mode for LTE");
  // if (_modem.setNetworkMode(38) > 0)
  // {
  //   if (_modem.gprsConnect(apn) > 0)
  //   {
  //     SerialMon.println("Connected!");
  //   }
  //   break;
  // }
  // else
  // {
  //   SerialMon.println("Failed!");
  // }

  // bool conn = _modem.isGprsConnected();
  return _modem.isGprsConnected();
  // SerialMon.println((conn) ? "Connected" : "Failed");
}

String SIM7600G::getGPS()
{
  if (SerialAT.available() == 0)
  {
    SerialAT.begin(defaultBaud, SERIAL_8N1, RX_GSM, TX_GSM, false);
  }

  if (_modem.enableGPS() && !gps_enabled)
  {
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

void SIM7600G::manualCommand()
{
  debug();
  SerialAT.println("ATI");
}