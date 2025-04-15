#include <Arduino.h>

class Encoder
{
private:
    uint8_t s1_pin, s2_pin, key_pin;
    bool s1_old, s2_old, key_old;

    uint8_t state;
    uint8_t is_hold_moving;
    uint32_t hold_timer;
    uint32_t hold_delay;
    bool no_hold;
    

    void (*leftFunc)();
    void (*rightFunc)();
    void (*leftHoldFunc)();
    void (*rightHoldFunc)();
    void (*pressFunc)();
    void (*holdFunc)();

    void leftAct(){
        if(!digitalRead(key_pin)){
            is_hold_moving = true;
        }
        if(is_hold_moving){
            leftHoldFunc();
        }
        else {
            leftFunc();
        }
    }
    void rightAct(){
        if(!digitalRead(key_pin)){
            is_hold_moving = true;
        }
        if(is_hold_moving){
            rightHoldFunc();
        }
        else {
            rightFunc();
        }
    }

public:
    Encoder(uint8_t s1_pin,
            uint8_t s2_pin,
            uint8_t key_pin,
            uint32_t hold_delay,
            void (*leftFunc)(),
            void (*rightFunc)(),
            void (*leftHoldFunc)(),
            void (*rightHoldFunc)(),
            void (*pressFunc)(),
            void (*holdFunc)())
    {
        this->s1_pin = s1_pin;
        this->s2_pin = s2_pin;
        this->key_pin = key_pin;
        this->hold_delay = hold_delay;
        this->leftFunc = leftFunc;
        this->rightFunc = rightFunc;
        this->leftHoldFunc = leftHoldFunc;
        this->rightHoldFunc = rightHoldFunc;
        this->pressFunc = pressFunc;
        this->holdFunc = holdFunc;
    }
    void init()
    {
        pinMode(s1_pin, INPUT);
        pinMode(s2_pin, INPUT);
        pinMode(key_pin, INPUT);
    }
    void update()
    {
        const bool s1 = !digitalRead(s1_pin);
        const bool s2 = !digitalRead(s2_pin);
        const bool key = !digitalRead(key_pin);

        if(key != key_old){
            key_old = key;
            if(key){
                hold_timer = millis();
            }
            if(!key){
                if(!no_hold && !is_hold_moving){
                    pressFunc();
                }
                is_hold_moving = false;
                no_hold = false;
            }
        }
        if(key && !is_hold_moving && !no_hold){
            if(millis() - hold_timer >= hold_delay){
                no_hold = true;
                holdFunc();
            }
        }

        if (s1 != s1_old ||
            s2 != s2_old)
        {
            switch (state)
            {
                // Starting point
                case 0:
                    if(s1){
                        state = 1;
                    }
                    else if(s2){
                        state = 4;
                    }
                    break;
                
                // Processing left action
                case 1:
                    if(!s1){
                        state = 0;
                    }
                    else if(s2){
                        state = 2;
                    }
                    break;
                case 2:
                    if(!s1 && !s2){
                        leftAct();
                        state = 0;
                    }
                    else if(!s2){
                        state = 1;
                    }
                    else if(!s1){
                        state = 3;
                    }
                break;
                case 3:
                    if(!s2){
                        leftAct();
                        state = 0;
                    }
                    else if(s1){
                        state = 2;
                    }
                break;

                // Processing right action
                case 4:
                    if(!s2){
                        state = 0;
                    }
                    else if(s1){
                        state = 5;
                    }
                    break;
                case 5:
                    if(!s1 && !s2){
                        rightAct();
                        state = 0;
                    }
                    else if(!s1){
                        state = 4;
                    }
                    else if(!s2){
                        state = 6;
                    }
                break;
                case 6:
                    if(!s1){
                        rightAct();
                        state = 0;
                    }
                    else if(s2){
                        state = 5;
                    }
                break;

            s1_old = s1;
            s2_old = s2;  
            }
        }
    }
};