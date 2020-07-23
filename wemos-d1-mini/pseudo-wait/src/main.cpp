#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266TimerInterrupt.h>

#define _high(x) digitalWrite(x, HIGH)
#define _low(x) digitalWrite(x, LOW)
#define _write(x, y) digitalWrite(x, y)
#define TIMER_INTERVAL_MS 100

const int CLK_D1 = D4;
const int CLK_D2 = D6;
const int CLK_D3 = D7;
const int CLK_D4 = D8;
const int CLK_A = TX;
const int CLK_B = D0;
const int CLK_C = D5;
const int CLK_D = D2;
const int CLK_E = D1;
const int CLK_F = RX;
const int CLK_G = D3;

/**
 * Update wifi credentials
 * */
const char *ssid = "<ssid>";
const char *password = "<password>";

ESP8266Timer ITimer;

/**
 * Turn off everything 
**/
void resetLegs()
{
  _high(CLK_A);
  _high(CLK_B);
  _high(CLK_C);
  _high(CLK_D);
  _high(CLK_E);
  _high(CLK_F);
  _high(CLK_G);
}

void reset()
{

  _low(CLK_D1);
  _low(CLK_D2);
  _low(CLK_D3);
  _low(CLK_D4);

  resetLegs();
}

byte DIGIT_PINS[] = {CLK_D1, CLK_D2, CLK_D3, CLK_D4};
byte NEXT_DIGIT = 0;

bool FLAG_WIFI_CONNECTED = false;

void ICACHE_RAM_ATTR nextDigit(void)
{
  if (FLAG_WIFI_CONNECTED == false)
  {
    reset();
    _high(DIGIT_PINS[NEXT_DIGIT]);
    _low(CLK_G);
    NEXT_DIGIT++;
    if (NEXT_DIGIT == 4)
    {
      NEXT_DIGIT = 0;
    }
  }
}


void onStationModeConnected(WiFiEventStationModeConnected pInfo)
{
  FLAG_WIFI_CONNECTED = true;
  // ITimer.detachInterrupt();
  // reset();
}

void onStationModeDisconnected(WiFiEventStationModeDisconnected pInfo)
{
  FLAG_WIFI_CONNECTED = false;
  // ITimer.attachInterrupt(TIMER_INTERVAL_MS, nextDigit);
}
void setup()
{

  // Serial.begin(9600);
  pinMode(RX, FUNCTION_3);
  pinMode(TX, FUNCTION_3);
  pinMode(CLK_A, OUTPUT);
  pinMode(CLK_B, OUTPUT);
  pinMode(CLK_C, OUTPUT);
  pinMode(CLK_D, OUTPUT);
  pinMode(CLK_E, OUTPUT);
  pinMode(CLK_F, OUTPUT);
  pinMode(CLK_G, OUTPUT);
  // pinMode(CLK_DP, OUTPUT);

  pinMode(CLK_D1, OUTPUT);
  pinMode(CLK_D2, OUTPUT);
  pinMode(CLK_D3, OUTPUT);
  pinMode(CLK_D4, OUTPUT);

  WiFi.mode(WIFI_STA);

  ITimer.attachInterrupt(TIMER_INTERVAL_MS, nextDigit);
  // ITimer.enableTimer();
  WiFi.onStationModeConnected(onStationModeConnected);
  WiFi.onStationModeDisconnected(onStationModeDisconnected);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
  // ITimer.detachInterrupt();
  reset();
  delay(5000);
}

void loop()
{
  if (FLAG_WIFI_CONNECTED)
  {
    _high(DIGIT_PINS[NEXT_DIGIT]);
    _low(CLK_A);
    NEXT_DIGIT++;
    if (NEXT_DIGIT == 4)
    {
      NEXT_DIGIT = 0;
    }
    reset();
  }
}