#include <Arduino.h>

class Encoder
{
private:
    uint8_t s1_pin, s2_pin, key_pin;
    bool s1_old, s2_old, key_old;

    uint8_t state;

    void (*leftFunc)();
    void (*rightFunc)();
    void (*leftHoldFunc)();
    void (*rightHoldFunc)();
    void (*pressFunc)();
    void (*holdFunc)();

public:
    Encoder(uint8_t s1_pin,
            uint8_t s2_pin,
            uint8_t key_pin,
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
        bool s1 = !digitalRead(s1_pin);
        bool s2 = !digitalRead(s2_pin);
        bool key = !digitalRead(key_pin);

        if (s1 != s1_old ||
            s2 != s2_old ||
            key != key_old)
        {
            s1_old = s1;
            s2_old = s2;
            key_old = key;

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
                        leftFunc();
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
                        leftFunc();
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
                        rightFunc();
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
                        rightFunc();
                        state = 0;
                    }
                    else if(s2){
                        state = 5;
                    }
                break;
            
            }
        }
    }
};