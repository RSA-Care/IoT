#include <Arduino.h>
#include <HardwareSerial.h>
#include <TinyGPSPlus.h>
#include "dht22/dht_custom_header.h"

// GPS Variable
#define RX 16
#define TX 17

HardwareSerial gps_serial(2);
TinyGPSPlus gps;
DHT22_class dht;

void setup()
{
  Serial.begin(115200);
  gps_serial.begin(9600, SERIAL_8N2, RX, TX);

  Serial.println("Setup completed =====|");
}

void loop()
{
  // DHT22 code
  dht.data();

  // GPS code
  if (gps_serial.available() > 0)
  {
    gps.encode(gps_serial.read());
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat());
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng());
  }
  else
  {
    Serial.println("[ ! ] GPS Serial unavailable.");
  }

  delay(5000);
}