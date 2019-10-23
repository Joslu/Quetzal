#include "IRremote.h"


//Define sensor pin
const int RECV_PIN = 10;
const int LED = 2;

int togglestate = 0;
//Define IR Receiver and Results objects

IRrecv irrecv(RECV_PIN);
decode_results results;

int s1 = 20;
int s2 = 19;
int s3 = 15;
int s4 = 14;
int s5 = 16;

void setup()
{
    Serial.begin(9600);
    irrecv.enableIRIn();
    pinMode(LED,OUTPUT);
    pinMode(s1,OUTPUT);
    pinMode(s2,OUTPUT);
    pinMode(s3,OUTPUT);
    pinMode(s4,OUTPUT);
    pinMode(s5,OUTPUT);

    

void loop()
{
    
    if (irrecv.decode(&results)){
        Serial.println(results.value, HEX);

        switch (results.value)
        {
        case 0xC9D1F64D:

            if(togglestate == 0){
                digitalWrite(LED,HIGH);
                togglestate = 1;
            }

            else{
                digitalWrite(LED,LOW);
                togglestate = 0;
            }

            break;
        
        default:
            break;
        }
        irrecv.resume();
    }
	
}
