//Teensy LC
//Serial + MIDI
//48 Mhz
//Smallest code

#include "ADC.h"
#include "Scheduler/Scheduler.h"
#include "Ribbon.h"
#include "Expression.h"
#include "SerialUI.h"

#define uint unsigned int
#define ulong unsigned long


#define led 13

Metro heart_beat = Metro(alive, 500);


void setup(){
    Serial.begin(0);
    
    pinMode(led, OUTPUT);
    
    heart_beat.start();
    
    Ribbon.setup();
    Ribbon.enable();
    
    Expression.setup();
    Expression.enable();
    
    digitalWrite(led, HIGH);
}

void loop(){
    databurn();
    MetroManager.update();
    SerialUI.update();
}

void alive(){
    static boolean last = true;
    digitalWrite(led, last);
    last = !last;
}

void databurn(){
    while(usbMIDI.read()); //Drop any incoming data
}
