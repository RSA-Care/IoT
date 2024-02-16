#include "4G/4G_handler.h"
#include "dht22/dht_custom_header.h"
#include "oled/oled.h"
#include <string.h>
#include <HardwareSerial.h>
#include <TinyGPSPlus.h>

#define defaultBaud 115200

// #include <TinyGsmClient.h>
// #include <StreamDebugger.h>
// #include <PubSubClient.h>