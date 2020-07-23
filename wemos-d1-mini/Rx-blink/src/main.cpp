#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:
  pinMode(RX, FUNCTION_3);
  pinMode(RX, OUTPUT);
  // pinMode(D2, OUTPUT);
  // digitalWrite(D2, HIGH);
}

void loop() {
  digitalWrite(RX, HIGH);
  delay(1000);
  digitalWrite(RX, LOW);
  delay(1000);
}