#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define _high(x) digitalWrite(x, HIGH)
#define _low(x) digitalWrite(x, LOW)
#define _write(x, y) digitalWrite(x, y)

#define NTP_DEBOUNCE 5000

const int CLK_D1 = D0;
const int CLK_D2 = D7;
const int CLK_D3 = D8;
const int CLK_D4 = D6;
const int CLK_A = TX;
const int CLK_B = D4;
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

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", 19800, 60000);

enum APP_STATE
{
  PROG_STATE_WIFI_NOT_CONNECTED,
  PROG_STATE_WIFI_CONNECTED,
  PROG_STATE_NTP_STARTED,
  PROG_STATE_WAIT
};

APP_STATE PROG_STATE;

void onSTAGotIP(WiFiEventStationModeGotIP evt)
{
  PROG_STATE = PROG_STATE_WIFI_CONNECTED;
}

void onSTADisconnected(WiFiEventStationModeDisconnected evt)
{
  PROG_STATE = PROG_STATE_WIFI_NOT_CONNECTED;
}

void onSTAConnected(WiFiEventStationModeConnected)
{
  // nothing to do...
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

  PROG_STATE = PROG_STATE_WAIT;

  WiFi.begin(ssid, password);

  static WiFiEventHandler e1 = WiFi.onStationModeGotIP(onSTAGotIP);
  static WiFiEventHandler e2 = WiFi.onStationModeConnected(onSTAConnected);
  static WiFiEventHandler e3 = WiFi.onStationModeDisconnected(onSTADisconnected);
}

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

void displayNumber(int position, uint8_t number)
{
  reset();
  _high(position);
  byte val = number & HIGH;
  _write(CLK_G, val);
  resetLegs();
  val = (number >> 1) & HIGH;
  _write(CLK_F, val);
  resetLegs();
  val = (number >> 2) & HIGH;
  _write(CLK_E, val);
  resetLegs();
  val = (number >> 3) & HIGH;
  _write(CLK_D, val);
  resetLegs();
  val = (number >> 4) & HIGH;
  _write(CLK_C, val);
  resetLegs();
  val = (number >> 5) & HIGH;
  _write(CLK_B, val);
  resetLegs();
  val = (number >> 6) & HIGH;
  _write(CLK_A, val);
  resetLegs();
}

/**
 * Refer the following google sheet - Time Bits:
 * 
 *  https://docs.google.com/spreadsheets/d/1_sjjd6l8fZ_HoLpbHEiR9KEUCEQ4YyOIuhMzEn7jl-M/edit?usp=sharing
 *
 * */
const uint16_t CLOCK_BYTES[] = {
    0b00000010000001, // 0
    0b00000011001111, // 1
    0b00000010010010, // 2
    0b00000010000110, // 3
    0b00000011001100, // 4
    0b00000010100100, // 5
    0b00000010100000, // 6
    0b00000010001111, // 7
    0b00000010000000, // 8
    0b00000010000100, // 9
    0b10011110000001, // 10
    0b10011111001111, // 11
    0b10011110010010, // 12
    0b10011110000110, // 13
    0b10011111001100, // 14
    0b10011110100100, // 15
    0b10011110100000, // 16
    0b10011110001111, // 17
    0b10011110000000, // 18
    0b10011110000100, // 19
    0b00100100000001, // 20
    0b00100101001111, // 21
    0b00100100010010, // 22
    0b00100100000110, // 23
    0b00100101001100, // 24
    0b00100100100100, // 25
    0b00100100100000, // 26
    0b00100100001111, // 27
    0b00100100000000, // 28
    0b00100100000100, // 29
    0b00001100000001, // 30
    0b00001101001111, // 31
    0b00001100010010, // 32
    0b00001100000110, // 33
    0b00001101001100, // 34
    0b00001100100100, // 35
    0b00001100100000, // 36
    0b00001100001111, // 37
    0b00001100000000, // 38
    0b00001100000100, // 39
    0b10011000000001, // 40
    0b10011001001111, // 41
    0b10011000010010, // 42
    0b10011000000110, // 43
    0b10011001001100, // 44
    0b10011000100100, // 45
    0b10011000100000, // 46
    0b10011000001111, // 47
    0b10011000000000, // 48
    0b10011000000100, // 49
    0b01001000000001, // 50
    0b01001001001111, // 51
    0b01001000010010, // 52
    0b01001000000110, // 53
    0b01001001001100, // 54
    0b01001000100100, // 55
    0b01001000100000, // 56
    0b01001000001111, // 57
    0b01001000000000, // 58
    0b01001000000100  // 59
};

const uint8_t BITMASK_7 = 0b1111111;

void showHours(uint16_t hours)
{
  uint8_t _LOW = hours & BITMASK_7;
  uint8_t _HIGH = (hours >> 7);
  displayNumber(CLK_D1, _HIGH);
  displayNumber(CLK_D2, _LOW);
}

void showMinutes(uint16_t minutes)
{
  uint8_t _LOW = minutes & BITMASK_7;
  uint8_t _HIGH = (minutes >> 7);
  displayNumber(CLK_D3, _HIGH);
  displayNumber(CLK_D4, _LOW);
}

void showTime(int hours, int minutes)
{
  showHours(CLOCK_BYTES[hours]);
  showMinutes(CLOCK_BYTES[minutes]);
}

byte WAIT_PINS[] = {CLK_D1, CLK_D2, CLK_D3, CLK_D4};
byte WAIT_PIN_INDEX = 0;
int hr = 0, mt = 0;
unsigned long last = 0;
void loop()
{
  if (PROG_STATE == PROG_STATE_WIFI_NOT_CONNECTED)
  {
    reset();
    WiFi.reconnect();
    PROG_STATE = PROG_STATE_WAIT;
  }
  else if (PROG_STATE == PROG_STATE_WIFI_CONNECTED)
  {
    timeClient.begin();
    // timeClient.forceUpdate();
    PROG_STATE = PROG_STATE_NTP_STARTED;
  }
  else if (PROG_STATE == PROG_STATE_NTP_STARTED)
  {
    if ((millis() - last > NTP_DEBOUNCE) && timeClient.update())
    {
      last = millis();
      hr = timeClient.getHours();
      mt = timeClient.getMinutes();
    }
    showTime(hr, mt);
  }
  else if (PROG_STATE == PROG_STATE_WAIT)
  {
    reset();
    _high(WAIT_PINS[WAIT_PIN_INDEX]);
    _low(CLK_G);
    WAIT_PIN_INDEX = (++WAIT_PIN_INDEX == 4) ? 0 : WAIT_PIN_INDEX;
    delay(500);
  }
}