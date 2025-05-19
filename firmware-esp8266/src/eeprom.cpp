#define REAL_EEPROM_DATA_MARKER 0x1984
#define DEFAULT_STEPPER_FREQ 500
#define DEFAULT_STEPPER_DIR 1
#define DEFAULT_GOAL_TEMP_C 200

#define MIN_TARGET_TEMP 100
#define MAX_TARGET_TEMP 200
#define MIN_STEPPER_FREQ 100
#define MAX_STEPPER_FREQ 2000

#include <Arduino.h>
#include <EEPROM.h>

class MachinaSettings
{
    private:
    int stepper_freq;
    bool stepper_dir;
    float target_temp;

    bool is_working;
    bool is_error;
    int error_code;
    bool is_default;
    bool is_updated;

    public:
    bool turnOn(){
        bool changed = !is_working;
        is_working = 1;
        return changed;
    }
    bool turnOff(){
        bool changed = is_working;
        is_working = 0;
        return changed;
    }
    bool getIsWorking(){
        return is_working;
    }
    bool errorNone(){
        bool changed = !is_error;
        is_error = 1;
        error_code = -1;
        return changed;
    }
    bool throwError(int error_code){
        bool changed = this->error_code != error_code;
        is_error = 1;
        this->error_code = error_code;
        return changed;
    }
    bool getIsError(){
        return is_error;
    }
    int getErrorCode(){
        return error_code;
    }
    bool clearError(){
        bool changed = is_error;
        is_error = 0;
        error_code = 0;
        return changed;
    }
    bool getIsDefault(){
        return is_default;
    }
    bool clearIsDefault(){
        bool changed = is_default;
        is_default = 0;
        return changed;
    }
    bool updateStepperDir(bool stepper_dir){
        bool changed = this->stepper_dir != stepper_dir;
        this->stepper_dir = stepper_dir;
        if(changed){
            is_updated = true;
            is_default = false;
        }
        return changed;
    }
    bool updateStepperFreq(int stepper_freq){
        bool changed = this-> stepper_freq != stepper_freq;
        if(stepper_freq >= MIN_STEPPER_FREQ && stepper_freq <= MAX_STEPPER_FREQ){
            this->stepper_dir = stepper_freq;
        }
        else {
            changed = 0;
        }
        if(changed){
            is_updated = true;
            is_default = false;
        }
        return changed;
    }
    bool updateTargetTemp(float target_temp){
        bool changed = this->target_temp != target_temp;
        if(target_temp >= MIN_TARGET_TEMP && target_temp <= MAX_TARGET_TEMP){
            this->target_temp = target_temp;
        }
        else {
            changed = 0;
        }
        if(changed){
            is_updated = true;
            is_default = false;
        }
        return changed;
    }
    bool getIsUpdated(){
        return is_updated;
    }
    float getTargetTemp(){
        return target_temp;
    }
    int getStepperFreq(){
        return stepper_freq;
    }
    bool getStepperDir(){
        return stepper_dir;
    }
    bool clearIsUpdated(){
        bool changed = is_updated;
        is_updated = 0;
        return changed;
    }
    static bool checkTargetTemp(float temp){
        return temp >= MIN_TARGET_TEMP && temp <= MAX_TARGET_TEMP;
    }
    static bool checkStepperFreq(int freq){
        return freq >= MIN_STEPPER_FREQ && freq <= MAX_STEPPER_FREQ;
    }
    void readEEPROM()
    {
        uint32_t marker;
        EEPROM.get(0, marker);
        is_updated = 1;
        if (marker == (uint32_t)REAL_EEPROM_DATA_MARKER)
        {
            this->is_default = false;

            uint32_t raw_stepper_freq;
            bool raw_stepper_dir;
            float raw_target_temp;

            EEPROM.get(sizeof(marker), raw_stepper_freq);
            EEPROM.get(sizeof(marker)+sizeof(raw_stepper_freq), raw_stepper_dir);
            EEPROM.get(sizeof(marker)+sizeof(raw_stepper_freq)+sizeof(raw_stepper_dir), raw_target_temp);

            if(checkStepperFreq(raw_stepper_freq) && checkTargetTemp(raw_target_temp)){
                this->stepper_freq = raw_stepper_freq;
                this->stepper_dir = raw_stepper_dir;
                this->target_temp = raw_target_temp;
                return;
            }
        }
        this->is_default = true;
        this->stepper_freq = DEFAULT_STEPPER_FREQ;
        this->stepper_dir = DEFAULT_STEPPER_DIR;
        this->target_temp = DEFAULT_GOAL_TEMP_C;
    }

    void saveEEPROM()
    {
        EEPROM.put(0, (uint32_t)REAL_EEPROM_DATA_MARKER); 
        EEPROM.put(sizeof((uint32_t)REAL_EEPROM_DATA_MARKER), this->stepper_freq);
        EEPROM.put(sizeof((uint32_t)REAL_EEPROM_DATA_MARKER)+
        sizeof(this->stepper_freq), this->stepper_dir);
        EEPROM.put(sizeof((uint32_t)REAL_EEPROM_DATA_MARKER)+
        sizeof(this->stepper_freq)+sizeof(this->stepper_dir),this->target_temp);
        EEPROM.commit();
    }
};
