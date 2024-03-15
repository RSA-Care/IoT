#include "4G/4G_handler.h"
#include "dht22/dht_custom_header.h"
#include "mqtt/mqtt_handler.h"
#include "_Antares/Antares_handler.h"
#include "oled/oled.h"
#include <string.h>
#include <HardwareSerial.h>
#include <TinyGPSPlus.h>

#define defaultBaud 115200

// #include <PubSubClient.h>