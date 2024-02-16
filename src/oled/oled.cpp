#include "oled.h"

Adafruit_SSD1306 _dis(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Oled::Oled()
{
  if (!_dis.begin())
  {
    Serial.println("Failed to initialize OLED!");
  }
  else
  {
    Serial.println("OLED Initialized.");
    _dis.display();
  }
}

void Oled::oledPrint(String text)
{
  _dis.clearDisplay();
  _dis.print(text);
}