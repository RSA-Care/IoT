#include "oled/oled.h"
#include "4G/4G_handler.h"
#include "dht22/dht_custom_header.h"
#include "mqtt/mqtt_handler.h"
#include "_Antares/Antares_handler.h"
#include "A9G/handler.h"
#include <string.h>
#include <HardwareSerial.h>

#define defaultBaud 115200

// #include <PubSubClient.h>