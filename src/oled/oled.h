#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_ADDRESS 0x3D
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

class Oled
{
private:
  void testScreen();
  void resetToTop();
  void printHeader();
  String headerName;
  bool nameChange = false;
  bool _state = true;

public:
  Oled();
  bool init();
  void setHeader(String name);
  void debug(String text);
  void alwaysPrintToDisplay(boolean state = true);
};
