#include <NTPClient.h>
// #include <ESP8266WiFi.h>
#include <WiFiUdp.h>
// #include <DNSServer.h>
// #include <ESP8266WebServer.h>
// #include <WiFiManager.h> //https://github.com/tzapu/WiFiManager
// #include "ESP8266TimerInterrupt.h"

// #define CLK_D1 1
// #define CLK_D2 2
// #define CLK_D3 3
// #define CLK_D4 4
// #define CLK_A 5
// #define CLK_B 6
// #define CLK_C 7
// #define CLK_D 8
// #define CLK_E 9
// #define CLK_F 10
// #define CLK_G 11
// #define CLK_DP 12

int CLK_D1 = D4;
int CLK_D2 = D6;
int CLK_D3 = D7;
int CLK_D4 = D8;
int CLK_A = TX;
int CLK_B = D0;
int CLK_C = D5;
int CLK_D = D2;
int CLK_E = A0;
int CLK_F = RX;
int CLK_G = D3;
int CLK_DP = D1;

/**
 * Turn off everything 
**/
void reset()
{

  digitalWrite(CLK_D1, LOW);
  digitalWrite(CLK_D2, LOW);
  digitalWrite(CLK_D3, LOW);
  digitalWrite(CLK_D4, LOW);

  digitalWrite(CLK_A, HIGH);
  digitalWrite(CLK_B, HIGH);
  digitalWrite(CLK_C, HIGH);
  digitalWrite(CLK_D, HIGH);
  digitalWrite(CLK_E, HIGH);
  digitalWrite(CLK_F, HIGH);
  digitalWrite(CLK_G, HIGH);
  digitalWrite(CLK_DP, HIGH);
}

const long HOURS[] = {
    0,
    0b10011110,        // 1
    0b00100110,        // 2
    0b00001100,        // 3
    0b10011000,        // 4
    0b01001000,        // 5
    0b01000000,        // 6
    0b00011110,        // 7
    0b00000000,        // 8
    0b00001000,        // 9
    0b100111100000011, // 10
    0b100111110011111, // 11
    0b100111100100111  // 12
};

const long MINUTES[] = {
    0b00000010000001, // 0
    0b00000011001111, // 1
    0b00000010010011, // 2
    0b00000010000110, // 3
    0b00000011001100, // 4
    0b00000010100100, // 5
    0b00000010100000, // 6
    0b00000010001111, // 7
    0b00000010000000, // 8
    0b00000010000100, // 9
    0b10011110000001, // 10
    0b10011111001111, // 11
    0b10011110010011, // 12
    0b10011110000110, // 13
    0b10011111001100, // 14
    0b10011110100100, // 15
    0b10011110100000, // 16
    0b10011110001111, // 17
    0b10011110000000, // 18
    0b10011110000100, // 19
    0b00100110000001, // 20
    0b00100111001111, // 21
    0b00100110010011, // 22
    0b00100110000110, // 23
    0b00100111001100, // 24
    0b00100110100100, // 25
    0b00100110100000, // 26
    0b00100110001111, // 27
    0b00100110000000, // 28
    0b00100110000100, // 29
    0b00001100000001, // 30
    0b00001101001111, // 31
    0b00001100010011, // 32
    0b00001100000110, // 33
    0b00001101001100, // 34
    0b00001100100100, // 35
    0b00001100100000, // 36
    0b00001100001111, // 37
    0b00001100000000, // 38
    0b00001100000100, // 39
    0b10011000000001, // 40
    0b10011001001111, // 41
    0b10011000010011, // 42
    0b10011000000110, // 43
    0b10011001001100, // 44
    0b10011000100100, // 45
    0b10011000100000, // 46
    0b10011000001111, // 47
    0b10011000000000, // 48
    0b10011000000100, // 49
    0b01001000000001, // 50
    0b01001001001111, // 51
    0b01001000010011, // 52
    0b01001000000110, // 53
    0b01001001001100, // 54
    0b01001000100100, // 55
    0b01001000100000, // 56
    0b01001000001111, // 57
    0b01001000000000, // 58
    0b01001000000100  // 59
};

WiFiUDP ntpUdp;
NTPClient timeClient(ntpUdp, "asia.pool.ntp.org", 19800, 60000);

int NEXT_DIGIT_PIN = CLK_D1;
unsigned long current;

// void doTimerTask()
// {
//   reset();
//   digitalWrite(NEXT_DIGIT_PIN, HIGH);
//   digitalWrite(CLK_DP, LOW);
//   if (NEXT_DIGIT_PIN == CLK_D1)
//   {
//     NEXT_DIGIT_PIN = CLK_D2;
//   }
//   else if (NEXT_DIGIT_PIN == CLK_D2)
//   {
//     NEXT_DIGIT_PIN = CLK_D3;
//   }
//   else if (NEXT_DIGIT_PIN == CLK_D3)
//   {
//     NEXT_DIGIT_PIN = CLK_D4;
//   }
//   else if (NEXT_DIGIT_PIN == CLK_D4)
//   {
//     NEXT_DIGIT_PIN = CLK_D1;
//   }
// }

// ESP8266Timer ITimer;
// #define TIMER_INTERVAL 100

void setup()
{
  pinMode(RX, FUNCTION_3);
  pinMode(TX, FUNCTION_3);

  /**
   * Commenting below 2 lines.
   * They would be set when
   * CLK_* is set, in the lines
   * following
   **/
  // pinMode(RX, OUTPUT);
  // pinMode(TX, OUTPUT);

  pinMode(CLK_A, OUTPUT);
  pinMode(CLK_B, OUTPUT);
  pinMode(CLK_C, OUTPUT);
  pinMode(CLK_D, OUTPUT);
  pinMode(CLK_E, OUTPUT);
  pinMode(CLK_F, OUTPUT);
  pinMode(CLK_G, OUTPUT);
  pinMode(CLK_DP, OUTPUT);

  pinMode(CLK_D1, OUTPUT);
  pinMode(CLK_D2, OUTPUT);
  pinMode(CLK_D3, OUTPUT);
  pinMode(CLK_D4, OUTPUT);

  // ITimer.attachInterruptInterval(TIMER_INTERVAL, doTimerTask);

  // while(true) {
  //   delay(1000);
  // }
}

void writeDigit(int digit, long number)
{
  reset();
  
}

void loop()
{
  // put your main code here, to run repeatedly:
}