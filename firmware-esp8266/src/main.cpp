#define ENC_S1 D6
#define ENC_S2 D5
#define ENC_KEY D7

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 4);

void debugEncoderOnSerial();

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Hello World!");

  Serial.begin(115200);
  pinMode(ENC_S1, INPUT);
  pinMode(ENC_S2, INPUT);
  pinMode(ENC_KEY, INPUT);
}

void loop() {
  debugEncoderOnSerial();
}

bool old_s1, old_s2, old_key;
void debugEncoderOnSerial(){
  bool s1 = !digitalRead(ENC_S1);
  bool s2 = !digitalRead(ENC_S2);
  bool key = !digitalRead(ENC_KEY);

  if(s1 != old_s1 ||
  s2 != old_s2 ||
  key != old_key){
    old_s1 = s1;
    old_s2 = s2;
    old_key = key;

    Serial.print((int)s1 + 4);
    Serial.print(", ");
    Serial.print((int)s2 + 2);
    Serial.print(", ");
    Serial.print((int)key + 0);
    Serial.println();
  }
}

