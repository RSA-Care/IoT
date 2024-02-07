#include <Arduino.h>
#include "AntaresESP8266HTTP.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

//Masukan Wifi details

#define WIFISSID ""
#define PASSWORD ""

//Masukan Antares Details

#define ACCESSKEY ""
#define projectName ""
#define deviceName ""

#define DHTTYPE DHT22

//Sesuaikan pin di board!
#define GPS_PIN1 1
#define GPS_PIN2 2
#define DHTPIN 3



AntaresESP8266HTTP antares(ACCESSKEY);
DHT dht (DHTPIN,DHTTYPE);
SoftwareSerial ss(GPS_PIN1,GPS_PIN2);

//Initiation
void temperature();
void gpsProcess();
int varSatu, varDua,varTiga, varEmpat; //placeholder variable, use wisely!
int temp;

void temperature() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
  Serial.println(F("Failed to read from DHT sensor!"));
  return;
  }
  temp = dht.computeHeatIndex(t, h, false);
  Serial.print(temp);
  Serial.print(F("°C "));
}

void gps(){
  while (ss.available() > 0){
    // get the byte data from the GPS
    byte gpsData = ss.read();
    Serial.write(gpsData);
  }
}

void setup() {
  Serial.begin(9600);
  ss.begin(9600);
  antares.setDebug(true);
  antares.wifiConnection(WIFISSID,PASSWORD);
  dht.begin();
}

void loop() {
  //Call necessary functions
  temperature();
  gps();

  // Add variable data to storage buffer
  antares.add("Sensor", "DHT22", temp);
  antares.add("identitas", "nama", varDua);
  antares.add("Sensor",varTiga);
  antares.add("Distance", varEmpat);


  // Send from buffer to Antares
  antares.send(projectName, deviceName);
}