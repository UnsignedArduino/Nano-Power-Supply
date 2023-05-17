#include <Arduino.h>
#include <Rotary.h>
#include <SPI.h>
#include <SevSegShift.h>

const uint8_t POT_MIN = 0;
const uint32_t MILLIVOLT_MIN = 1280;
const uint8_t POT_MAX = 187;
const uint32_t MILLIVOLT_MAX = 24000;

const uint8_t SHIFT_PIN_SHCP = 6;
const uint8_t SHIFT_PIN_STCP = 7;
const uint8_t SHIFT_PIN_DS = 8;

const uint8_t ENCODER_A = 2;
const uint8_t ENCODER_B = 3;

const uint8_t POT_CS = 10;

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
int16_t prevValue = -1;

void writePotValue(uint8_t value) {
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  digitalWrite(POT_CS, LOW);

  SPI.transfer(value);

  digitalWrite(POT_CS, HIGH);
  SPI.endTransaction();
}

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

  SPI.begin();
  pinMode(POT_CS, OUTPUT);
  digitalWrite(POT_CS, HIGH);

  writePotValue(value);
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
  value = constrain(value, POT_MIN, POT_MAX);
  if (value != prevValue) {
    writePotValue(value);
    const float volts =
      map(value, POT_MIN, POT_MAX, MILLIVOLT_MIN, MILLIVOLT_MAX) / 1000.0;
    display.setNumberF(volts, volts > 10 ? 1 : 2);
    prevValue = value;
  }
}
