#include <Arduino.h>
#include <Rotary.h>
#include <SevSegShift.h>

const uint8_t SHIFT_PIN_SHCP = 6;
const uint8_t SHIFT_PIN_STCP = 7;
const uint8_t SHIFT_PIN_DS = 8;

const uint8_t ENCODER_A = 2;
const uint8_t ENCODER_B = 3;

const uint8_t numDigits = 4;
const uint8_t digitPins[] = {A1, A2, A3, A4};
const uint8_t segmentPins[] = {0, 2, 4, 6, 7, 1, 3, 5};
const bool resistorsOnSegments = false;
const uint8_t hardwareConfig = COMMON_CATHODE;
const bool updateWithDelays = false;
const bool leadingZeros = false;
const bool disableDecPoint = false;

SevSegShift display(SHIFT_PIN_DS, SHIFT_PIN_SHCP, SHIFT_PIN_STCP, 1, true);

Rotary encoder = Rotary(ENCODER_A, ENCODER_B);

volatile int16_t value = 0;

void setup() {
  display.begin(hardwareConfig, numDigits, (uint8_t*)digitPins,
                (uint8_t*)segmentPins, resistorsOnSegments, updateWithDelays,
                leadingZeros, disableDecPoint);
  display.setBrightness(90);
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  OCR1A = 125;
  TCCR1B = (1 << WGM12) | (1 << CS12);
  TIMSK1 = (1 << OCIE1A);

  encoder.begin();
  PCICR |= (1 << PCIE2);
  PCMSK2 |= (1 << PCINT18) | (1 << PCINT19);
  sei();
}

ISR(TIMER1_COMPA_vect) {
  display.refreshDisplay();
}

ISR(PCINT2_vect) {
  const uint8_t result = encoder.process();
  if (result == DIR_NONE) {

  } else if (result == DIR_CW) {
    value++;
  } else if (result == DIR_CCW) {
    value--;
  }
}

void loop() {
  if (value > 9999) {
    value = 0;
  } else if (value < 0) {
    value = 9999;
  }

  display.setNumber(value);
}
