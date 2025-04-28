#define REAL_EEPROM_DATA_MARKER 0x1984
#define DEFAULT_STEPPER_FREQ 500
#define DEFAULT_STEPPER_DIR 1
#define DEFAULT_GOAL_TEMP_C 200

#include <Arduino.h>
#include <EEPROM.h>

struct MachinaSettings
{
    uint32_t stepper_freq;
    bool stepper_dir;
    float target_temp;

    bool is_working;
    bool is_error;
    bool is_default;
    bool is_updated;

    void readEEPROM()
    {
        uint32_t marker;
        EEPROM.get(0, marker);
        if (marker == (uint32_t)REAL_EEPROM_DATA_MARKER)
        {
            this->is_default = false;

            uint32_t raw_stepper_freq;
            bool raw_stepper_dir;
            float raw_target_temp;

            EEPROM.get(sizeof(marker), raw_stepper_freq);
            EEPROM.get(sizeof(marker)+sizeof(raw_stepper_freq), raw_stepper_dir);
            EEPROM.get(sizeof(marker)+sizeof(raw_stepper_freq)+sizeof(raw_target_temp), raw_target_temp);

            this->stepper_freq = raw_stepper_freq;
            this->stepper_dir = raw_stepper_dir;
            this->target_temp = raw_target_temp;
        }
        else
        {
            this->is_default = true;
            this->stepper_freq = DEFAULT_STEPPER_FREQ;
            this->stepper_dir = DEFAULT_STEPPER_DIR;
            this->target_temp = DEFAULT_GOAL_TEMP_C;
        }
    }

    void saveEEPROM()
    {
            EEPROM.put(0, (uint32_t)REAL_EEPROM_DATA_MARKER); 
            EEPROM.put(sizeof((uint32_t)REAL_EEPROM_DATA_MARKER), this->stepper_freq);
            EEPROM.put(sizeof((uint32_t)REAL_EEPROM_DATA_MARKER)+
            sizeof(this->stepper_freq), this->stepper_dir);
            EEPROM.put(sizeof((uint32_t)REAL_EEPROM_DATA_MARKER)+
            sizeof(this->stepper_freq)+sizeof(this->target_temp),this->target_temp);
        EEPROM.commit();
    }
};
