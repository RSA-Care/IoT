#include "handler.h"

unsigned long startTime = 0;
unsigned long duration = 5000;

HardwareSerial a9gSerial(2);

a9g::a9g() {}

void a9g::parseData(String buff)
{
  Serial.println(buff);
}

bool a9g::begin()
{
  a9gSerial.begin(defaultBaud);

  Serial.print("Checking Serial AT..");

  startTime = millis();
  while (a9gSerial.available() == 0 && (millis() - startTime < duration))
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println();

  if (a9gSerial.available())
  {
    a9gSerial.println("AT+GPS=1");
    a9gSerial.println("AT+GPSRD=1");
    delay(3000);
    parseData(a9gSerial.readString());
    return true;
  }

  return false;
}

String a9g::getGPS()
{
  a9gSerial.println("AT+LOCATION=1");
  parseData(a9gSerial.readString());

  return a9gSerial.readString();
}