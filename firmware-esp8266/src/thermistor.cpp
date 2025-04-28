#include <Arduino.h>
class Thermistor{
    private:
    uint8_t pin;
    double nominal_resistance;
    double nominal_temperature;
    double beta;
    double rRef;
    uint16_t sampling_rate;
    std::vector<int> samples;

    public:
    Thermistor(uint8_t pin,
    double nominal_resistance,
    double nominal_temperature,
    double beta,
    double rRef,
    uint16_t sampling_rate
    )
    {
        this->pin=pin;
        this->nominal_resistance = nominal_resistance;
        this->nominal_temperature = nominal_temperature;
        this->beta = beta;
        this-> rRef = rRef;
        //this->samples = int[sampling_rate];
    }
    void init(){
        pinMode(pin, INPUT);
    }
};