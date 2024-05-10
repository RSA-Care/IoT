
#include "package.h"

#define _SSID "Amirty"
#define _PASS "Viol@17A"

// #define _SSID "bahagia permai"
// #define _PASS "imut2023"

Oled display;
DHT22_class dht;
// SIM7600G modem;
MQTTHandler mqtt(_SSID, _PASS);
Antares antares;
a9g sim;

struct _dat
{
  float lat = 0;
  float lon = 0;
  float temp = 0;
  float hum = 0;
};

void setup()
{
  Serial.begin(defaultBaud);
  Serial.println("\n=== > Setup Start < ===\n");

  // Oled Init
  display.init();

  // A9G
  if (sim.begin())
  {
    Serial.println("A9G Connected.");
  }
  else
  {
    Serial.println("A9G Error: Failed to connect\nPlease check wiring.");
  }

  display.setHeader("WiFi");
  display.println("Connecting to WiFi. Please wait...");
  if (mqtt.setupWIFI())
  {
    // Serial.println("WiFi Connected!");
    display.debug(F("Connected to WiFi!"));
    IPAddress IP = WiFi.localIP();
    String SSID_NAME = WiFi.SSID();
    display.debug("SSID : " + SSID_NAME);
    display.debug("LOCAL IP : ");
    display.debug(IP.toString());
    delay(5000);
  }
  else
  {
    // Serial.println("WiFi connection Failed!");
    display.debug(F("Failed to connect to WiFi!"));
  }

  display.setHeader("SIM7600G");
  display.debug("Initializing...");
  // bool status4G = modem.init();
  // display.debug(status4G ? F("Ready") : F("Fail"));
  // display.debug("SIM7600G Initialized.");

  // display.setHeader(F("Antares"));
  // antares.init();

  display.setHeader(F("MQTT Broker."));
  int counter = 0;
  display.print(F("Connecting MQTT Server..."));
  while (!mqtt.MQTTconnect() && counter < 5)
  {
    display.print(".");
    counter++;
    delay(500);
  }
  display.println();

  display.println(mqtt.getStatus());

  delay(5000);
  // Serial.println("\n|===== Setup completed =====|");
  display.setHeader(F("Setup completed!"));
  display.debug("===== Completed =====");
}

void loop()
{
  // Temp data for testing
  float temp, hum;
  float longitude, latitude;

  temp = 30;
  hum = 25;
  longitude = -6.9692767;
  latitude = 107.6255821;

  Serial.println("Sending data.");
  mqtt.publish(longitude, latitude, temp, hum);
  delay(5000);
}