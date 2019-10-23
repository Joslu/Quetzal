#ifndef Motors_h
#define Motors_h
#include "Arduino.h"


class Motors
{
    public:
        //Constructor where is conected
        Motors(int pinAIN1, int pinAIN2, int pin_PWMA, int pin_BIN1, int pin_BIN2, int pin_PWMB, int pin_ENABLE);
        
        enableMotors();
        disableMotors();
        
        LeftMotor(int speed);
        RightMotor(int speed);

        Drive(int speedL, int speedR);
        Brake(bool left, bool rigt, int speed);


       

    private:
        int _pin_AIN1, _pin_AIN2, _pin_PWMA, _pin_BIN1, _pin_BIN2, _pin_PWMB, _pin_ENABLE;
        


};






#endif