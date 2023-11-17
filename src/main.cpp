// clang-format off
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_INA260.h>
// clang-format on

LiquidCrystal_PCF8574 lcd(0x27);

Adafruit_INA260 ina260 = Adafruit_INA260();

void setup() {
  Serial.begin(9600);
  Serial.println("Nano power supply");
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  lcd.begin(16, 2);
  lcd.home();
  lcd.clear();
  lcd.setBacklight(true);

  if (!ina260.begin()) {
    Serial.println("Could not find INA260");
    lcd.println("Error!");
    lcd.println("Sensor missing");
  }
}

void loop() {
  static bool pleaseUpdate = false;
  static uint32_t lastUpdate = 0;

  float voltage = ina260.readBusVoltage() / 1000;

  if (pleaseUpdate || millis() - lastUpdate > 200) {
    pleaseUpdate = false;
    lastUpdate = millis();
    lcd.home();
    lcd.print("V: ");
    lcd.print(voltage);
    lcd.print(" v ");
  }
}
