#include <Arduino.h>

const uint8_t SHIFT_CS = 9;
const uint8_t SHIFT_CLK = 10;
const uint8_t SHIFT_DATA = 8;

const uint8_t digitTable[] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9
};

void setup() {
  Serial.begin(9600);

  pinMode(SHIFT_CS, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_DATA, OUTPUT);
}

void displayDigit(uint8_t num) {
  digitalWrite(SHIFT_CS, LOW);
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, digitTable[num]);
  digitalWrite(SHIFT_CS, HIGH);
}

void loop() {
  for (uint8_t i = 0; i < 10; i++) {
    displayDigit(i);
    delay(500);
  }
}
