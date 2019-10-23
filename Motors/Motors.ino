//H-Bridge pins

#include "src/Motors.h"

int pinPWMA = 5;
int pinAIN1 = 3;
int pinAIN2 = 4;

int pinBIN1 = 7;
int pinBIN2 = 8;
int pinPWMB = 9;
int pinSTBY = 6;



Motors MotorsRobot(pinAIN1,pinAIN2,pinPWMA,pinBIN1,pinBIN2, pinPWMB, pinSTBY);


void setup()
{
    Serial.begin(9600);
    

    MotorsRobot.enableMotors();
}

void loop()
{


    Serial.println("Giro");
	MotorsRobot.LeftMotor(255);
    delay(5000);
    Serial.println("Invierte");
	MotorsRobot.LeftMotor(-255);
    delay(5000);
    Serial.println("Reduce");
	MotorsRobot.LeftMotor(-95);
    delay(5000);
    MotorsRobot.Brake(HIGH,HIGH,100);
    delay(1000);
  


}
