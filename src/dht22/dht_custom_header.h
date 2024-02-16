#include <DHT.h>
#include <DHT_U.h>

class DHT22_class
{
private:
  struct fData
  {
    bool success;
    float temperature, humidity, heatIndex;
  };
  fData data;

  void printDHT(String text);

public:
  DHT22_class();
  fData getData();
};