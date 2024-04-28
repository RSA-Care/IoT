#include "mqtt_handler.h"

// const char *test_mqtt_server = "192.168.1.23";
// const char *mqtt_user = "admin";
// const char *mqtt_pass = "hivemq";

const char *test_mqtt_server = "http://f6dea6546f094ce29468848d85af5038.s1.eu.hivemq.cloud";
const char *mqtt_user = "satria";
const char *mqtt_pass = "gogombc23";

const char *mqtt_server = "broker.hivemq.com";

// const int port = 1883; // default mqtt port
const int port = 8883; // TLS/SSL Port

WiFiClient WirelessClient;
PubSubClient _mqtt_client(WirelessClient);

MQTTHandler::MQTTHandler(const char *SSID, const char *PASS) : _SSID(SSID), _PASS(PASS), useWIFI(false) {}

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
      delay(500);
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
  WiFi.begin(_SSID, _PASS); // Connect to it with the credentials we got from the config.json
  delay(1000);

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
  {
    Serial.println("Failed to connect to WiFi. Restarting...");
    checkWIFI();
    return useWIFI;
  }

  useWIFI = true;
  return useWIFI;
}

bool MQTTHandler::MQTTconnect(boolean test)
{
  if (test)
  {
    Serial.println("Using public MQTT broker.");
    _mqtt_client.setServer(mqtt_server, 1883);
    Serial.print("Using MQTT server : ");
    Serial.println(mqtt_server);
  }
  else
  {
    _mqtt_client.setServer(test_mqtt_server, port);
    Serial.print("Using MQTT server : ");
    Serial.println(test_mqtt_server);
    Serial.print("Port : ");
    Serial.println(port);
  }

  String clientId = "ESP8266Client-"; // Create a random client ID
  clientId += String(random(0xffff), HEX);
  _mqtt_client.connect(clientId.c_str(), mqtt_user, mqtt_pass);

  Serial.println("ClientID : " + clientId);
  Serial.print("Username : ");
  Serial.println(mqtt_user);

  delay(1000);

  /* ERROR STATUS:

  -4 : MQTT_CONNECTION_TIMEOUT - the server didn't respond within the keepalive time
  -3 : MQTT_CONNECTION_LOST - the network connection was broken
  -2 : MQTT_CONNECT_FAILED - the network connection failed
  -1 : MQTT_DISCONNECTED - the client is disconnected cleanly
  0 : MQTT_CONNECTED - the client is connected
  1 : MQTT_CONNECT_BAD_PROTOCOL - the server doesn't support the requested version of MQTT
  2 : MQTT_CONNECT_BAD_CLIENT_ID - the server rejected the client identifier
  3 : MQTT_CONNECT_UNAVAILABLE - the server was unable to accept the connection
  4 : MQTT_CONNECT_BAD_CREDENTIALS - the username/password were rejected
  5 : MQTT_CONNECT_UNAUTHORIZED - the client was not authorized to connect
  */
  int state = _mqtt_client.state();

  switch (state)
  {
  case -4:
    errorStatus = "MQTT_CONNECTION_TIMEOUT - the server didn't respond within the keepalive time";
    break;

  case -3:
    errorStatus = "MQTT_CONNECTION_LOST - the network connection was broken";
    break;

  case -2:
    errorStatus = "MQTT_CONNECT_FAILED - the network connection failed";
    break;

  case -1:
    errorStatus = "MQTT_DISCONNECTED - the client is disconnected cleanly";
    break;

  case 0:
    errorStatus = "MQTT_CONNECTED - the client is connected";
    break;

  case 1:
    errorStatus = "MQTT_CONNECT_BAD_PROTOCOL - the server doesn't support the requested version of MQTT";
    break;

  case 2:
    errorStatus = "MQTT_CONNECT_BAD_CLIENT_ID - the server rejected the client identifier";
    break;

  case 3:
    errorStatus = "MQTT_CONNECT_UNAVAILABLE - the server was unable to accept the connection";
    break;

  case 4:
    errorStatus = "MQTT_CONNECT_BAD_CREDENTIALS - the username/password were rejected";
    break;

  case 5:
    errorStatus = "MQTT_CONNECT_UNAUTHORIZED - the client was not authorized to connect";
    break;

  default:
    break;
  }

  Serial.print("Client State : ");
  Serial.println(errorStatus);

  return _mqtt_client.connected();
}

String MQTTHandler::getStatus()
{
  return errorStatus;
}