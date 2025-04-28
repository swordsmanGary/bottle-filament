#include <Arduino.h>

class Stepper{
    private:
    uint8_t en_pin;
    uint8_t dir_pin;
    uint8_t step_pin;
    uint32_t freq;

    public:
    Stepper(
        uint8_t en_pin,
        uint8_t dir_pin,
        uint8_t step_pin)
    {
        this->en_pin = en_pin;
        this->dir_pin = dir_pin;
        this->step_pin = step_pin;
    }
    void init(){
        pinMode(en_pin,OUTPUT);
        digitalWrite(en_pin, 0);
        pinMode(dir_pin,OUTPUT);
        pinMode(step_pin,OUTPUT);
    }
    void disable(){
        digitalWrite(en_pin, 1);
    }
    void enable(){
        digitalWrite(en_pin, 0);
    }
    void setFreq(uint32_t freq){
        this->freq = freq;
        analogWriteFreq(freq);
        analogWrite(step_pin, 100);
    }
    void setDir(bool dir){
        digitalWrite(dir_pin, dir);
    }
};