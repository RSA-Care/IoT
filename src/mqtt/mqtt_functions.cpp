#include "mqtt_handler.h"

// const char *test_mqtt_server = "192.168.1.23";
// const char *mqtt_user = "admin";
// const char *mqtt_pass = "hivemq";

const char *public_broker = "broker.hivemq.com";

const char *mqtt_server = "35.224.42.35";
// const char *mqtt_server = "http://f6dea6546f094ce29468848d85af5038.s1.eu.hivemq.cloud";
const char *mqtt_user = "admin";
const char *mqtt_pass = "hivemq";

const char *topic = "info";

const int port = 1883; // default mqtt port
                       // const int port = 8883; // TLS/SSL Port

WiFiClient WirelessClient;
PubSubClient _mqtt(WirelessClient);

MQTTHandler::MQTTHandler(const char *SSID, const char *PASS) : _SSID(SSID), _PASS(PASS), useWIFI(false) {}

bool MQTTHandler::MQTTconnect(boolean test)
{
  if (test)
  {
    // Testing using public broker
    Serial.print("Broker : ");
    Serial.println(public_broker);
    Serial.println("Connecting..");
    _mqtt.setServer(public_broker, port);
  }
  else
  {
    Serial.print("Broker : ");
    Serial.println(mqtt_server);
    Serial.println("Connecting..");
    _mqtt.setServer(mqtt_server, port);
  }

  clientId = "ESP32Client-"; // Create a random client ID
  clientId += String(random(0xffff), HEX);
  _mqtt.connect(clientId.c_str(), mqtt_user, mqtt_pass);

  String state = stateCheck();
  Serial.println("\n=== MQTT State ===");
  Serial.println(state);
  Serial.println("==================\n");

  return _mqtt.connected();
}

String MQTTHandler::stateCheck()
{
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
  int state = _mqtt.state();

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

  return errorStatus;
}

void MQTTHandler::reconnect()
{
  while (!_mqtt.connected())
  {
    Serial.println("Reconnecting to MQTT Broker...");
    if (_mqtt.connect(clientId.c_str(), mqtt_user, mqtt_pass))
    {
      Serial.println("Connected!");
    }
    else
    {
      Serial.print("Error: ");
      stateCheck();
      Serial.println(errorStatus);
    }
  }
}

bool MQTTHandler::publish(float longitude, float latitude, float temperature, float humidity)
{
  if (!_mqtt.connected())
  {
    reconnect();
  }
  String payload = String(longitude) + "," + String(latitude) + "," + String(temperature) + "," + String(humidity);
  return _mqtt.publish("test", payload.c_str(), true);
}