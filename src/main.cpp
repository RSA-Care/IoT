
#include "package.h"

TinyGPSPlus gps;
DHT22_class dht;
SIM7600G modem;
mqtt_handler mqtt;
Antares antares;
Oled display;

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
  // display.alwaysPrintToDisplay();

  int counter = 0;
  display.setHeader("SIM7600G");
  display.debug("Initializing...");
  while (!modem.init() && counter < 5)
  {
    display.debug(F("Modem init failed, retrying..."));
    delay(1000);
    // Serial.print(F("[ "));
    // Serial.print(counter);
    // Serial.println(" ] Connection failed, restarting!");
    counter++;
  }

  display.debug("Connecting to WiFi...");
  if (!modem.getNetworkStatus())
  {
    if (mqtt.setupWIFI())
    {
      // Serial.println("WiFi Connected!");
      display.debug(F("Connected to WiFi!"));
    }
    else
    {
      // Serial.println("WiFi connection Failed!");
      display.debug(F("Failed to connect to WiFi!"));
    }
  }

  display.setHeader(F("Antares"));
  antares.init();

  delay(2000);
  // Serial.println("\n|===== Setup completed =====|");
  display.setHeader(F("Setup completed!"));
}

void loop()
{
  _dat data;

  // DHT22 code
  auto dhtData = dht.getData();
  String gps = modem.getGPS();
  data.temp = dhtData.temperature;
  data.hum = dhtData.humidity;
  data.lat = -6;
  data.lon = 75;

  antares.publish(true);

  // mqtt.publish(gps, dhtData.temperature, dhtData.temperature);

  Serial.println(gps);
  delay(5000);
}