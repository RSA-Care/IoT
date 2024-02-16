
#include "package.h"

TinyGPSPlus gps;
DHT22_class dht;
SIM7600G modem;

void setup()
{
  Serial.begin(defaultBaud);
  Serial.println("\n=== > Setup Start < ===\n");
  while (!modem.init(true) > 0)
  {
    Serial.println("Connection failed, restarting!");
  }
  modem.manualCommand();

  delay(500);
  Serial.println("\n|===== Setup completed =====|");
}

void loop()
{
  // DHT22 code
  auto dhtData = dht.getData();

  // GPS code
  // while (gpsSerial.available() > 0)
  // {
  //   if (gps.encode(gpsSerial.read()))
  //   {
  //     Serial.print("Latitude: ");
  //     Serial.println(gps.location.lat());
  //     Serial.print("Longitude: ");
  //     Serial.println(gps.location.lng());
  //     Serial.write(gpsSerial.read());
  //   }

  //   if (millis() > 5000 && gps.charsProcessed() < 10)
  //   {
  //     Serial.println(F("[ Neo 7m ] No GPS detected: check wiring."));
  //   }

  //   auto dhtData = dht.getData();

  //   delay(5000);
  // }

  Serial.println("GPS Serial unavailable.");
  delay(5000);
}