#include "handler.h"

bool a9g::begin()
{
  a9gSerial.begin(defaultBaud);
  a9gSerial.println("AT");
}