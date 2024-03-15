#include "oled.h"

Adafruit_SSD1306 _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Oled::Oled()
{
}

void Oled::testScreen()
{
  Serial.println("Testing display");
  _display.clearDisplay();

  _display.setTextSize(1);              // Normal 1:1 pixel scale
  _display.setTextColor(SSD1306_WHITE); // Draw white text
  // _display.setCursor(0, 12);            // Start at top-left corner
  _display.cp437(true); // Use full 256 char 'Code Page 437' font

  _display.write("Hello, World!"); // Write a string to the display
  // _display.drawLine(0, 10, 128, 10, SSD1306_WHITE); // Draw a horizontal line

  _display.display();
  delay(2000);
}

void Oled::printHeader()
{
  delay(1000);
  _display.clearDisplay();
  _display.setCursor(0, 0);
  _display.print("# ");
  _display.print(headerName);
  _display.setCursor(0, 12);
}

void Oled::resetToTop()
{
  if (_display.getCursorY() > SCREEN_HEIGHT)
  {
    _display.clearDisplay();
    // _display.setCursor(0, 0);
    printHeader();
  }
}

bool Oled::init()
{
  boolean initialize = _display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  if (!initialize)
  {
    Serial.println(F("SSD1306 allocation failed"));
    Serial.println(F("Please check for wire."));
  }
  delay(2000);
  _display.display();
  testScreen();
  delay(2000);

  _display.clearDisplay();
  return initialize;
}

/*
  WORK IN PROGRESS...

  > GOAL: Create a function that will display the output of serial to the display.
          This should be able to handle multiple lines and scroll through them as they come in.
*/
void Oled::alwaysPrintToDisplay(boolean state)
{
  Serial.print("Always Print To Display State: ");
  Serial.println(state ? "true" : "false");

  _state = state;

  _display.setCursor(0, 0);
  _display.print("Before loop");

  while (state)
  {
    _display.setTextSize(1);
    char c = Serial.read();
    _display.print(c);
    _display.write(_display.getCursorY());
    _display.display();
    resetToTop();
    delay(500);
  }
}

void Oled::setHeader(String name)
{
  headerName = name;
  printHeader();
}

void Oled::debug(String text)
{
  resetToTop();
  Serial.println(text);
  _display.println(text);
  _display.display();
  delay(1000);
}