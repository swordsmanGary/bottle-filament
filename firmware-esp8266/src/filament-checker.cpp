#include <Arduino.h>

class Checker{
    private:
    uint8_t pin;
    bool old_state = 1;
    void (*func)();

    public:
    Checker(
        int8_t pin,
        void (*func)()
    )
    {
        this->pin = pin;
        this->func = func;
    }
    void init(){
        pinMode(pin, INPUT_PULLUP);
    }
    void update(){
        bool state = !digitalRead(pin);        
        if(old_state != state){
            if(state){
                func();
            }
            old_state = state;
        }
    }
};