#include "mqtt_handler.h"

mqtt_handler::mqtt_handler()
{
}

// Private Methods
bool mqtt_handler::checkWIFI()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    useWIFI = true;
  }
  else
  {
    useWIFI = false;
  }

  return useWIFI;
}

// Public Methods
bool mqtt_handler::setupWIFI()
{
  Serial.println("Setting up WiFi Connection.");
  WiFi.mode(WIFI_STA);      // Set the ESP
  WiFi.begin(_SSID, _PASS); // Connect to it with the credentials we got from the config.json

  int counter = 0;
  Serial.print("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED && counter < 15)
  {
    Serial.print(".");
    delay(500);
    counter++;
  }
  Serial.println();

  if (WiFi.status() != WL_CONNECTED)
    return useWIFI;

  useWIFI = true;
  return useWIFI;
}