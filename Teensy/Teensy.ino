//Teensy LC
//Serial
//48 Mhz
//Smallest code

#include "Scheduler/Scheduler.h"

#define uint unsigned int
#define ulong unsigned long


#define led 13

Metro heart_beat;


void setup(){
    Serial.begin(0);
    
    pinMode(led, OUTPUT);
    
    heart_beat = Metro(alive, 500);
    heart_beat.start();
    
    digitalWrite(led, HIGH);
}

void loop(){
    MetroManager.update();
}

void alive(){
    static boolean last = true;
    digitalWrite(led, last);
    last = !last;
}