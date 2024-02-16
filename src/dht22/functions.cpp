#include "dht_custom_header.h"

#define pin 13
#define type DHT22

DHT _dht(pin, type);

DHT22_class::DHT22_class()
{
  _dht.begin();
}

void DHT22_class::printDHT(String text)
{
  Serial.print("[ DHT22 ] ");
  Serial.println(text);
  return;
}

DHT22_class::fData DHT22_class::getData()
{
  printDHT("Checking DHT22...");
  float humidity = _dht.readHumidity();
  float temperature = _dht.readTemperature();

  if (isnan(humidity) || isnan(temperature))
  {
    printDHT("DHT22 not found.");
    printDHT("Please check wiring.");
    printDHT("Delaying 10 second.");
    data.success = false;
    delay(10000);
    return data;
  }
  else
  {
    float heatIndex = _dht.computeHeatIndex(temperature, humidity, false);
    data.temperature = temperature;
    data.humidity = humidity;
    data.heatIndex = heatIndex;
    data.success = true;

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
    return data;
  }
}