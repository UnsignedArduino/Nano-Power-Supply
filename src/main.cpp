#include <Arduino.h>
#include <SevSegShift.h>

const uint8_t SHIFT_PIN_SHCP = 6;
const uint8_t SHIFT_PIN_STCP = 7;
const uint8_t SHIFT_PIN_DS = 8;

const uint8_t numDigits = 4;
const uint8_t digitPins[] = {5, 4, 3, 2};
const uint8_t segmentPins[] = {0, 2, 4, 6, 7, 1, 3, 5};
const bool resistorsOnSegments = true;
const uint8_t hardwareConfig = COMMON_CATHODE;
const bool updateWithDelays = false;
const bool leadingZeros = false;
const bool disableDecPoint = false;

SevSegShift display(SHIFT_PIN_DS, SHIFT_PIN_SHCP, SHIFT_PIN_STCP, 1, true);

void setup() {
  display.begin(hardwareConfig, numDigits, (uint8_t*)digitPins,
                (uint8_t*)segmentPins, resistorsOnSegments, updateWithDelays,
                leadingZeros, disableDecPoint);
  display.setBrightness(90);
}

void loop() {
  static uint32_t timer = millis();
  static uint16_t decaS = 0;

  if (millis() - timer >= 10) {
    timer += 10;
    decaS++;

    if (decaS == 10000) {
      decaS = 0;
    }
    display.setNumber(decaS, 2);
  }

  display.refreshDisplay();
}
