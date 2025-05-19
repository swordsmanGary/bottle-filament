#define TEMP_NOMINAL_RESISTANCE 100000
#define TEMP_NOMINAL_TEMPERATURE 25
#define TEMP_BETA 3950
#define TEMP_REFERECNE_RESISTANCE 1095
#define TEMP_SAMPLING_BUFFER_SIZE 100
#define TEMP_PIN A0

#define HEAT_PIN D3
#define HEAT_UPDATE_DELAY_MS 500

#define STEPPER_STEP D1
#define STEPPER_EN D7
#define STEPPER_DIR D2

#define FILAMENT_DETECTOR_PIN D6

#define ERROR_OVERHEAT 1

#include <Arduino.h>

#include <EEPROM.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "filament-checker.cpp"
#include "heater.cpp"
#include "stepper.cpp"
#include "thermistor.cpp"
#include "eeprom.cpp"
#include "html.cpp"

const char* ssid = "Hello";
const char* password = "11111111";

void handleSerialInput();
void startMachina();
void stopMachina();
void endFilamentAction();
void overheatAction();
void handleRoot();
void handleUpdate();
void handleWrite();

ESP8266WebServer server(80);

MachinaSettings state = MachinaSettings();

Checker check = Checker(
  FILAMENT_DETECTOR_PIN,
  endFilamentAction
);

Heater heat = Heater(
  HEAT_PIN,
  overheatAction
);

Stepper step = Stepper(
  STEPPER_EN,
  STEPPER_DIR,
  STEPPER_STEP
);

Thermistor temp = Thermistor(
  TEMP_PIN,
  TEMP_NOMINAL_RESISTANCE,
  TEMP_NOMINAL_TEMPERATURE,
  TEMP_BETA,
  TEMP_REFERECNE_RESISTANCE,
  TEMP_SAMPLING_BUFFER_SIZE
);

void setup() {
  step.init();
  step.disable();
  check.init();
  heat.init();
  temp.init();

  EEPROM.begin(512);
  state.readEEPROM();

  startMachina();

  Serial.begin(115200);

  /*WiFi.begin(ssid, password);
  Serial.println("Підключення до WiFi...");
  while(WiFi.status() != WL_CONNECTED){
    delay(1);
  }*/
  WiFi.softAP(ssid, password);
  Serial.print("Підключено! IP адреса: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/update", HTTP_POST, handleUpdate);
  server.on("/write", HTTP_POST, handleWrite);

  server.begin();
  Serial.println("Сервер запущено");
}

uint32_t heater_timer;

void loop() {
  server.handleClient();
  handleSerialInput();
  /*if(state.is_updated){
    state.is_updated = false;
    step.setFreq(state.stepper_freq);
    step.setDir(state.stepper_dir);
    heat.target_temp = state.target_temp;
    if(state.is_working){
      heat.heatOn();
    } else {
      heat.heatOff();
    }
  }
  if(state.is_working){
    if(millis() - heater_timer >= HEAT_UPDATE_DELAY_MS){
      heater_timer = millis();
      heat.update(temp.get());
    }
    check.update();
  }*/
}
void handleRoot(){
  server.send(200, "text/html", index_html);
}

void handleUpdate(){
  if(server.hasArg("onoff")){
    (bool)server.arg("onoff").toInt() ? state.turnOn() : state.turnOff();
  }
  if(server.hasArg("speed")){
    state.updateStepperFreq(server.arg("speed").toInt());
  }
  if(server.hasArg("dir")){
    state.updateStepperDir((bool)server.arg("dir").toInt());
  }
  if(server.hasArg("temp")){
    state.updateTargetTemp(server.arg("temp").toFloat());
  }
  Serial.println("Дані оновлено");

  server.send(200, "text/plain", "Дані успішно оновлено");

}
void handleWrite(){
  state.saveEEPROM();
 
  Serial.println("Дані збережено");
  server.send(200, "text/plain", "Дані успішно збережено");
}

void handleSerialInput(){
  if(Serial.available()>0){
    String str = Serial.readString();
    String rawValue = str.substring(1, str.length()-1);
    str.trim();
    switch(str[0]){
      case 'e':
        startMachina();
        Serial.println("Turned on");
        break;
      case 's':
        stopMachina();
        Serial.println("Turned off");
        break;
      case 'f':
        state.updateStepperFreq(rawValue.toInt());
        Serial.print("Now stepper frequency is ");
        Serial.println(state.getStepperFreq());
        break;
      case 'd':
        state.updateStepperDir(rawValue.toInt());
        Serial.print("Now stepper direction is ");
        Serial.println(state.getStepperDir());
        break;
      case 't':
        state.updateTargetTemp(rawValue.toFloat());
        Serial.print("Now target temperature is ");
        Serial.println(state.getTargetTemp());
        break;
      case 'p':
        Serial.print("Stepper frequency: ");
        Serial.println(state.getStepperFreq());
        Serial.print("Stepper direction: ");
        Serial.println(state.getStepperDir());
        Serial.print("Target temperature: ");
        Serial.println(state.getTargetTemp());
        Serial.print("Is settings default: ");
        Serial.println(state.getIsDefault());
        break;
      case 'w':
        state.saveEEPROM();
        Serial.println("Data is written into EEPROM");
        break;
      case 'r':
        state.readEEPROM();
        Serial.println("Data was read from EEPROM");
        break;
    }
    Serial.println(str);
  }
}
void startMachina(){
  state.turnOn();
  step.setFreq(state.getStepperFreq());
  step.enable();
  heat.heatOn();
  Serial.println("startMachina");
}
void stopMachina(){
  state.turnOff();
  step.disable();
  heat.heatOff();
  Serial.println("stopMachina");
}
void endFilamentAction(){
  stopMachina();
  Serial.println("endFilamentAction");
}
void overheatAction(){
  state.throwError(ERROR_OVERHEAT);
  stopMachina();
  Serial.println("overheatAction");
}