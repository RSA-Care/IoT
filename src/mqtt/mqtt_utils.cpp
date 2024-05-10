#include "mqtt_handler.h"

// Private Methods
bool MQTTHandler::checkWIFI()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    useWIFI = true;
  }
  else
  {
    useWIFI = false;
    int counter = 0;
    Serial.print("Connecting to WiFi..");
    while (WiFi.status() != WL_CONNECTED && counter < 20)
    {
      Serial.print(".");
      delay(1000);
      counter++;
    }
    Serial.println();

    useWIFI = WiFi.status() == WL_CONNECTED ? true : false;
  }

  return useWIFI;
}

// Public Methods
bool MQTTHandler::setupWIFI()
{
  Serial.println("Setting up WiFi Connection.");
  WiFi.mode(WIFI_STA); // Set the ESP
  WiFi.disconnect();
  delay(1000);
  Serial.print(F("SSID: "));
  Serial.println(_SSID);
  WiFi.begin(_SSID, _PASS); // Connecting to Wireless Network
  delay(1000);

  int counter = 0;
  Serial.print("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED && counter < 15)
  {
    Serial.print(".");
    delay(1000);
    counter++;
  }
  Serial.println();

  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Failed to connect to WiFi. Restarting...");
    checkWIFI();
    return useWIFI;
  }

  useWIFI = true;
  return useWIFI;
}

void callback(char *topic, byte *payload, unsigned int length)
{

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  String ipAddress = WiFi.localIP().toString();
  // client.publish(Communication2, ipAddress.c_str());
}

String MQTTHandler::getStatus()
{
  return errorStatus;
}