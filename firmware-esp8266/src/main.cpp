#define ENC_S1 D6
#define ENC_S2 D5
#define ENC_KEY D7

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 4);


void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Hello World!");

  Serial.begin(115200);
}

void loop() {
}