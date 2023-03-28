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

}

void loop() {
  
  uploadData(200, 12, 5, 120, 38);
  delay(5000);
}


void uploadData(int intensitas, float tegangan, float arus, int humidity, int temperature) {
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
