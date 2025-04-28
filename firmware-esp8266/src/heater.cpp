#include <Arduino.h>

class Heater
{
private:
    uint8_t pin;
    bool is_heating;

public:
    float target_temp;
    Heater(
        uint8_t pin,
        void (*overheatFunc)())
    {
        this->pin = pin;
    }
    void init(){
        pinMode(pin, OUTPUT);
    }
    void heatOn(){
        is_heating = true;
        digitalWrite(pin, 1);
    }
    void heatOff(){
        is_heating = false;
        digitalWrite(pin, 0);
    }
    void update(float current_temp){
        if(is_heating){
        }
    }
};