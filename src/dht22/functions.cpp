#include "dht_custom_header.h"

#define pin 13
#define type DHT22

DHT _dht(pin, type);

DHT22_class::DHT22_class()
{
  _dht.begin();
}

void DHT22_class::data()
{
  Serial.println("[ DHT22 ] Checking humidity, temperature, and heat index.");
  float humidity = _dht.readHumidity();
  float temperature = _dht.readTemperature();

  if (isnan(humidity) || isnan(temperature))
  {
    Serial.println("[ DHT22 ] Failed to detect humidity and temperature.");
    Serial.println("[ DHT22 ] Please check connections or DHT22 device.");
    Serial.println("[ DHT22 ] Delaying for 10 second");
    delay(10000);
    return;
  }
  else
  {
    float heatIndex = _dht.computeHeatIndex(temperature, humidity, false);
    Serial.println("[ DHT22 ] Success, displaying data:");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println("%");

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println("℃");

    Serial.print("Heat Index: ");
    Serial.print(heatIndex);
    Serial.println("℃");

    Serial.println("[ DHT22 ] === End of DHT22 data ===");
    delay(5000);
    return;
  }
}