//Teensy LC
//Serial
//48 Mhz
//Smallest code

#include "Scheduler.h"

#define uint unsigned int
#define ulong unsigned long


#define led 13

void alive();
void ping();

Metro heart_beat (alive, 500);

Metro eve[10];

void setup(){
    Serial.begin(0);
    while (!Serial.dtr());
    
    for(uint i = 0; i < 10; i++){
        eve[i].init(&ping, i + 500);
        eve[i].start();
    }
    
    heart_beat.start();
    
    pinMode(led, OUTPUT);
    digitalWrite(led, HIGH);
    Serial.println("Done");
}

void loop(){
    MetroManager()->update();
}

void ping(){
    Serial.println(millis());
}

void alive(){
    static boolean last = true;
    digitalWrite(led, last);
    last = !last;
}