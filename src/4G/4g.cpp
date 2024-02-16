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

  delay(3000);

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

  while (SerialAT.available() > 0)
  {
    SerialMon.println("SerialAT Ready!");

    SerialMon.println("Initializing SIM7600G...");
    _modem.restart();

    SerialMon.print("Sim Card Status : ");
    SerialMon.println(_modem.getSimStatus());

    String modemInfo = _modem.getModemInfo();
    SerialMon.print("Modem Info : ");
    SerialMon.println(modemInfo);

    _modem.sendAT("+SGPIO=0,4,1,1");
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

    SerialMon.println("Connecting to GPRS...");
    _modem.gprsConnect(apn);
    SerialAT.println("AT+CIPOPEN?");
    String input = SerialAT.readString();
    Serial.printf("AT+CIPOPEN?: %s \n", input.c_str());
    // if (_modem.gprsConnect(apn))
    // {
    //   SerialMon.println("GPRS Connected.");
    //   break;
    // }
    // else
    // {
    //   SerialMon.println("==> Retrying...\n");
    //   SerialMon.write(SerialAT.read());
    //   delay(10000);
    // }
  }

  // bool conn = _modem.isGprsConnected();
  return _modem.isNetworkConnected();
  // SerialMon.println((conn) ? "Connected" : "Failed");
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