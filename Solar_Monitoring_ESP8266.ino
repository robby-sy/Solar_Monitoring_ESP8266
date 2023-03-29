//==============================================wifi & http================================================//
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char *WIFI_SSID = "BARA";
const char *WIFI_PASSWORD = "224466RO";
const char *URL = "http://solardata.my.id/Update.php";

WiFiClient client;
HTTPClient httpClient;
//=========================================================================================================//

//==============================================DHT Sensor================================================//
#include "DHT.h"
#define DHTPIN 14 // pin D5 pada esp8266;
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
float h,t;
//=========================================================================================================//

//=============================================lux meter===================================================//
#include <BH1750.h>
#include <Wire.h>
BH1750 lightMeter;
float lux;
//=========================================================================================================//

void setup() {
  Serial.begin(9600);
  //=====================Menyambungkan esp8266 ke wifi=============================//
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");
  //==============================================================================//

  dht.begin(); // mulai DHT
  Wire.begin(); // mulai i2c connection untuk sensor intentsitas
  lightMeter.begin(); //mulai sensor intensitas cahaya
}

void loop() {
  //=================================membaca kelembapan dan suhu=================//
  h = dht.readHumidity();
  t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    h = 0;
    t = 0;}
  //=============================================================================//

  //=================================membaca intensitas cahaya===================//
  lux = lightMeter.readLightLevel();
  //=============================================================================//
  
  uploadData(lux, 12, 5, h, t);
  delay(5000);
}


void uploadData(int intensitas, float tegangan, float arus, float humidity, float temperature) {
  String data = "tegangan=";
  data += String(tegangan);
  data += "&intensitas=";
  data += String(intensitas);
  data += "&arus=";
  data += String(arus);
  data += "&kelembapan=";
  data += String(humidity);
  data += "&suhu=";
  data += String(temperature);
  httpClient.begin(client, URL);
  httpClient.addHeader("Content-Type", "application/x-www-form-urlencoded");
  httpClient.POST(data);
  String content = httpClient.getString();
  httpClient.end();
  Serial.println(content);
}
