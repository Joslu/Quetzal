#include "Arduino.h"
#include "Motors.h"

Motors::Motors(int pinAIN1, int pinAIN2, int pinPWMA, int pinBIN1, int pinBIN2, int pinPWMB, int pinENABLE){
    _pin_AIN1 = pinAIN1;
    _pin_AIN2 = pinAIN2;
    _pin_PWMA = pinPWMA;
    _pin_BIN1 = pinBIN1;
    _pin_BIN2 = pinBIN2;
    _pin_PWMB = pinPWMB;
    _pin_ENABLE = pinENABLE;

    pinMode(_pin_AIN1,OUTPUT);
    pinMode(_pin_AIN2,OUTPUT);
    pinMode(_pin_BIN1,OUTPUT);
    pinMode(_pin_BIN2,OUTPUT);
    pinMode(_pin_PWMA,OUTPUT);
    pinMode(_pin_PWMB,OUTPUT);
    pinMode(_pin_ENABLE,OUTPUT);
    
}

Motors::enableMotors(){

    digitalWrite(_pin_ENABLE, HIGH);
}

Motors::disableMotors(){
    digitalWrite(_pin_ENABLE, LOW);
}

Motors::LeftMotor(int speed){
    
    if (speed >= 0){
        digitalWrite(_pin_BIN1,HIGH);
        digitalWrite(_pin_BIN2,LOW);
    }

    else
    {
        digitalWrite(_pin_BIN1,LOW);
        digitalWrite(_pin_BIN2,HIGH);
        speed *= -1;
    }

    analogWrite(_pin_PWMB, speed);
    


}

Motors::RightMotor(int speed){

//Controla al motor derecho velocidad negativa -> atras y positiva -> adelante 
     if (speed >= 0){
        digitalWrite(_pin_AIN1,HIGH);
        digitalWrite(_pin_AIN2,LOW);
    }

    else
    {
        digitalWrite(_pin_AIN1,LOW);
        digitalWrite(_pin_AIN2,HIGH);
        speed *= -1;
    }

    analogWrite(_pin_PWMA, speed);

}

Motors::Drive(int speedL, int speedR){
    LeftMotor(speedL);
    RightMotor(speedR);
}

Motors::Brake(bool left, bool right, int speed){

    if (left)
    {
        digitalWrite(_pin_BIN1,HIGH);
        digitalWrite(_pin_BIN2,HIGH);
        analogWrite(_pin_PWMB,speed);
    }

    if (right)
    {
        digitalWrite(_pin_AIN1,HIGH);
        digitalWrite(_pin_AIN2,HIGH);
        analogWrite(_pin_PWMA,speed);
    }
}