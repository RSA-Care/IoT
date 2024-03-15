#include "Antares_handler.h"

AntaresESP32MQTT _antares(ACCESSKEY);

Antares::Antares()
{
}

bool Antares::init()
{
  // _antares.setDebug(true);
  // _antares.wifiConnection(_SSID, _PASS);
  _antares.setMqttServer();
  _antares.checkMqttConnection();

  return true;
}

void Antares::addData(_dat data)
{
  lon = data.lon;
  temp = data.temp;
  hum = data.hum;
  lat = data.lat;
  Serial.println(temp);
}

bool Antares::publish(boolean test)
{
  if (test)
  {
    hum = random(75, 90);
    temp = random(15, 24);
    lon = random(86, 134);
    lat = random(35, 45);
  }

  _antares.add("lat", lat);
  _antares.add("lon", lon);
  _antares.add("temp", temp);
  _antares.add("hum", hum);

  _antares.publish(PROJECT, DEVICE);
  delay(2000);

  return true;
}