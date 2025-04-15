#define ENC_S1 D6
#define ENC_S2 D5
#define ENC_KEY D7
#define ENC_HOLD_DELAY_MS 1000

#include <Arduino.h>

#include "encoder.cpp"
#include "filament-checker.cpp"
#include "heater.cpp"
#include "stepper.cpp"
#include "thermistor.cpp"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

void nothing();
void left();
void right();
void leftHold();
void rightHold();
void press();
void hold();

LiquidCrystal_I2C lcd(0x27, 16, 4);
Encoder enc(ENC_S2, ENC_S1, ENC_KEY, ENC_HOLD_DELAY_MS, left, right, leftHold, rightHold, press, hold);

void setup() {
  enc.init();
  lcd.init();

  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Hello World!");

  Serial.begin(115200);
}

void loop() {
  enc.update();
}

void left(){
  Serial.println("left");
}
void right(){
  Serial.println("right");
}
void leftHold(){
  Serial.println("leftHold");
}
void rightHold(){
  Serial.println("rightHold");
}
void press(){
  Serial.println("press");
}
void hold(){
  Serial.println("hold");
}
void nothing(){
}