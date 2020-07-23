#include <NTPClient.h>
// change next line to use with another board/shield
#include <ESP8266WiFi.h>
//#include <WiFi.h> // for WiFi shield
//#include <WiFi101.h> // for WiFi 101 shield or MKR1000
#include <WiFiUdp.h>

/**
 * Update wifi credentials
 * */
const char *ssid     = "<ssid>";
const char *password = "<password>";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", 19800);

void setup(){
  Serial.begin(9600);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
}

void loop() {
  timeClient.update();

  // Serial.println(timeClient.getFormattedTime());
  char dispString[20];
  sprintf(dispString, "hr: %d, min: %d", timeClient.getHours(), timeClient.getMinutes());
  Serial.println(dispString);
  delay(1000);
}