#include <IRremote.h>
#include <TimerOne.h>
#include "src/Motors.h"

//Constants, speed and time for motors

#define SPEED_SEARCH 150
#define SPEED_ATTACK 255
#define SPEED_TURN  100
#define SPEED_STOP 0
#define SPEED_BACK 130
#define SPEED_TEST 150
#define TIME_BACK 200
#define TIME_FRONT 200
#define TIME_TURN 200

//States of the system
#define S_HOME 0
#define S_STARTED 1
#define S_STOPED 2
#define S_TEST_SENSORS 3
#define S_TEST_MOTORS 4

//Buttons related to IRControl
#define b_HOME 0xFF18E7
#define b_START 0xFF38C7
#define b_STOP 0xFF4AB5
#define b_TEST_SENSORS 0xFFA25D
#define b_TEST_MOTORS 0xFF629D

//Sensores IR
#define S1 20
#define S2 19
#define S3 15
#define S4 14
#define S5 16

// Sensres piso
#define SP1 21
#define SP2 18

//Motores pines
int pinPWMA = 5;
int pinAIN1 = 3;
int pinAIN2 = 4;

int pinBIN1 = 7;
int pinBIN2 = 8;
int pinPWMB = 9;
int pinSTBY = 6;


volatile int state = S_HOME; //Estado actual lo colocamos en paro


enum class EnemyPosition {ONLEFT, ONCENTER, ONRIGHT, NOTFOUND};
EnemyPosition enemy_position = EnemyPosition::NOTFOUND;

// value of IR Sharp
const int ENEMY = 100;
const int ATTACK = 220;

//******Variables***********
// IR Sharp sensors
unsigned char IRsensors = 0;

//Define IR CONTROL pin
const int RECV_PIN = 10;

//debugging Led
const int LED = 2;

//
int lineValues[2];

//Objects definitios
Motors MotorsRobot(pinAIN1,pinAIN2,pinPWMA,pinBIN1,pinBIN2, pinPWMB, pinSTBY);

//Define IR Receiver and Results objects
IRrecv irrecv(RECV_PIN);

decode_results results;

void BlynkLed(int, int);

void setup()
{
	Serial.begin(9600);

    // set timer ISR
    Timer1.initialize(100000); // 10Hz
    Timer1.attachInterrupt(checkControl);
    MotorsRobot.enableMotors();

    irrecv.enableIRIn();
    
    pinMode(LED,OUTPUT);
    pinMode(S1,INPUT);
    pinMode(S2,INPUT);
    pinMode(S3,INPUT);
    pinMode(S4,INPUT);
    pinMode(S5,INPUT);
    pinMode(SP1,INPUT);
    pinMode(SP2,INPUT);

 

}

void loop() {
  
  //Modos de operación de robot 
  // HOME = Un estado de espera
  // STARTED = El robot empieza combate
  // STOPED = Se activa cuando se tenga que para el robot 

  switch(state){

    case S_HOME:

  
    Serial.println("HOME");
    BlynkLed(2,500);
   
    break;

    case S_STARTED:
    
    checkControl();
    Serial.println("S_STARTED");
    digitalWrite(LED,LOW);
    delay(5000);
    MotorsRobot.Drive(SPEED_SEARCH,SPEED_SEARCH);
    checkLine();
    motion();
    
    break;


    case S_STOPED:
    
    Serial.println("STOPED");
    digitalWrite(LED,HIGH);
    MotorsRobot.Brake(HIGH,HIGH,100);
   
        
   
    break;

    case S_TEST_MOTORS:

    TestMotors();

    break;

    case S_TEST_SENSORS:

    TestSensors();

    break;
          
    
    }

}



//Función asociada a la interrupción para el uso del control remoto 

void checkControl(){
    
    if (irrecv.decode(&results))
    {
        Serial.println(results.value, HEX);
        delay(10);
         
            switch (results.value)
            {
            case b_HOME:
                state = S_HOME;
                break;
            
            case b_START:
                state = S_STARTED;
                break;
            
            case b_STOP:
                state = S_STOPED;
                break;

            case b_TEST_MOTORS:
                state = S_TEST_MOTORS;
                break;
            
            case b_TEST_SENSORS:
                state = S_TEST_SENSORS;
                break;
            
            default:
                break;
            }
     
        irrecv.resume();

    }

}

//Funcion para darle visión al robot (Sensores Infrarojos)
void Eyes(){

    IRsensors = digitalRead(S1) + digitalRead(S2)*2 + digitalRead(S3)*4 + digitalRead(S4)*8 + digitalRead(S5)*16;
    switch(IRsensors)
  {
    case 0b00010001: EnemyPosition::ONCENTER; break;
    case 0b00011101: EnemyPosition::ONLEFT;   break;
    case 0b00011001: EnemyPosition::ONCENTER;  break; 
    case 0b00010011: EnemyPosition::ONCENTER;  break; 
    case 0b00010111: EnemyPosition::ONRIGHT;  break; 
    case 0b00011110: EnemyPosition::ONLEFT;  break; 
    case 0b00001111: EnemyPosition::ONRIGHT;  break;   
    default:  EnemyPosition::NOTFOUND; 
  }
}

//Movimientos del robot de acuerdo a que detecta
void motion(){

    Eyes();
    
    switch (enemy_position) {

      case EnemyPosition::NOTFOUND:
           Serial.print("NOTFOUND");
            //RSearching();
          break;  
      
      case EnemyPosition::ONCENTER:
             // RAttack();
          break;

      case EnemyPosition::ONLEFT:
            //RTurnI();
          break;

      case EnemyPosition::ONRIGHT:
          //RTurnR();         
          break;
    }

}

void checkLine(){

    lineValues[0] = analogRead(SP1);
    lineValues[1] = analogRead(SP2);
    delay(10);

    if (lineValues[0] >= 100 && lineValues[1] >= 100){

        MotorsRobot.Drive(-SPEED_TEST,-SPEED_TEST);
        delay(TIME_BACK);
        MotorsRobot.Drive(-SPEED_TURN, SPEED_TURN);
        delay(TIME_TURN);
        MotorsRobot.Drive(SPEED_STOP,SPEED_STOP);


    }
    else if (lineValues[0] >= 100 && lineValues[1] <= 100){
        //Se detecta a la izquierda
    }

    else if (lineValues[0] >= 100 && lineValues[1] <= 100){
        //Se detecta a la derecha
    }


}

void BlynkLed(int n, int tm){
    
    for (int i = 1; i <= n; i++){
        digitalWrite(LED,LOW);
        delay(tm);
        digitalWrite(LED,HIGH);
        delay(tm);
    }

    
}

void TestMotors(){
    Serial.println("MOTORS TEST");
    MotorsRobot.Drive(SPEED_TEST, SPEED_TEST);
    delay(TIME_FRONT);
    MotorsRobot.Drive(-SPEED_TEST,-SPEED_TEST);
    delay(TIME_BACK);

}

void TestSensors(){
    Serial.begin(9600);
        Serial.print("Sharp sensors: ");
        Serial.print(digitalRead(S5));
        Serial.print(digitalRead(S4));
        Serial.print(digitalRead(S3));
        Serial.print(digitalRead(S2));
        Serial.print(digitalRead(S1));
        Serial.print(" ");
        Serial.print("Line sensors: ");
        Serial.print(analogRead(SP1));
        Serial.print(" ");
        Serial.println(analogRead(SP2));
        delay(500);


}


// Movemente functions
void RSearching(){

}

void RAttack(){

}

void RTurnI(){

}

void RTurnR(){

}