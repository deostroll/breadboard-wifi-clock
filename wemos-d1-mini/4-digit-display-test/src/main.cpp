#include <Arduino.h>

#define DISPLAY_DELAY 1000
#define _high(x) digitalWrite(x, HIGH)
#define _low(x) digitalWrite(x, LOW)
#define _write(x, y) digitalWrite(x, y)

const uint8_t DIGIT[] = {
    0b0000001, // 0
    0b1001111,    // 1
    0b0010010, // 2
    0b0000110, // 3
    0b1001100, // 4
    0b0100100, // 5
    0b0100000, // 6
    0b0001111, // 7
    0b0000000, // 8
    0b0000100  // 9
};

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
// const int CLK_DP = D1;

/**
 * Turn off everything 
**/
void resetLegs() {

  digitalWrite(CLK_A, HIGH);
  digitalWrite(CLK_B, HIGH);
  digitalWrite(CLK_C, HIGH);
  digitalWrite(CLK_D, HIGH);
  digitalWrite(CLK_E, HIGH);
  digitalWrite(CLK_F, HIGH);
  digitalWrite(CLK_G, HIGH);
  // digitalWrite(CLK_DP, HIGH);
}

void reset()
{

  digitalWrite(CLK_D1, LOW);
  digitalWrite(CLK_D2, LOW);
  digitalWrite(CLK_D3, LOW);
  digitalWrite(CLK_D4, LOW);

  resetLegs();  
}



const byte k = 0x1;

void displayNumber(int position, uint8_t number) 
{
  reset();
  _high(position);
  byte val = number & k;
  _write(CLK_G, val);
  // delay(20);
  resetLegs();
  val = (number >> 1) & k;
  _write(CLK_F, val);
  // delay(20);
  resetLegs();
  val = (number >> 2) & k;
  _write(CLK_E, val);
  // delay(20);
  resetLegs();
  val = (number >> 3) & k;
  _write(CLK_D, val);
  // delay(20);
  resetLegs();
  val = (number >> 4) & k;
  _write(CLK_C, val);
  // delay(20);
  resetLegs();
  val = (number >> 5) & k;
  _write(CLK_B, val);
  // delay(20);
  resetLegs();
  val = (number >> 6) & k;
  _write(CLK_A, val);
  // delay(20);
  resetLegs();
}

long current = 0;

void setup()
{  
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
  current = millis();
}

const int DIG_SEQ[] = { CLK_D1, CLK_D2, CLK_D3, CLK_D4 };
int CUR_IDX = 0;
byte CUR_NUM_IDX = 0;

void loop()
{
  displayNumber(DIG_SEQ[CUR_IDX], DIGIT[CUR_NUM_IDX]);

  if((millis() - current) >= DISPLAY_DELAY) 
  {
    current = millis();        
    if(CUR_NUM_IDX == 9) {
      CUR_NUM_IDX = 0;     
      CUR_IDX = (CUR_IDX < 3) ? ++CUR_IDX : 0;      
      return;
    }
    CUR_NUM_IDX++;    
  }
  
  // Test individual digits
  // displayNumber(CLK_D1, DIGIT[6]);
}