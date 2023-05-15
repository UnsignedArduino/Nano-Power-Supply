#include <Arduino.h>

const uint8_t SHIFT_CS = 9;
const uint8_t SHIFT_CLK = 10;
const uint8_t SHIFT_DATA = 8;

const uint8_t SEGMENT_NUM = 4;
const uint8_t SEGMENT_CONTROL[SEGMENT_NUM] = {7, 6, 5, 4};

const uint8_t DIGIT_TABLE[] = {
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

void beginDigits() {
  pinMode(SHIFT_CS, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_DATA, OUTPUT);
  for (uint8_t i = 0; i < SEGMENT_NUM; i++) {
    pinMode(SEGMENT_CONTROL[i], OUTPUT);
    digitalWrite(SEGMENT_CONTROL[i], LOW);
  }
}

void displayDigit(uint8_t num, uint8_t pos, bool dp = false, uint16_t d = 5) {
  digitalWrite(SHIFT_CS, LOW);
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST,
           DIGIT_TABLE[num] | (dp ? 0b10000000 : 0));
  digitalWrite(SHIFT_CS, HIGH);
  digitalWrite(SEGMENT_CONTROL[pos], HIGH);
  delay(d);
  digitalWrite(SEGMENT_CONTROL[pos], LOW);
}

void setup() {
  Serial.begin(9600);

  beginDigits();
}

void loop() {
  for (uint8_t i = 0; i < 4; i++) {
    for (uint8_t j = 0; j < 10; j++) {
      displayDigit(j, i, false, 500);
    }
  }
}
