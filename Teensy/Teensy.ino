//Teensy LC

#include "ADC.h"
#include "Settings.h"
#include "Scheduler/Scheduler.h"
#include "Ribbon.h"
#include "SerialUI.h"

#define uint unsigned int
#define ulong unsigned long


#define led 13




//ControlChannel expr (MIDI_CH, CC_EXPRESSION_CTRL, CC_MODE_HIGH_RES);
Metro heart_beat (alive, 500);


void setup(){
    Serial.begin(0);
    
    pinMode(led, OUTPUT);
    
    heart_beat.start();
    Ribbon.setup();
    Ribbon.enable();
}

void loop(){
    databurn();
    MetroManager.update();
    SerialUI.update();
}

void alive(){
    static boolean last = false;
    digitalWrite(led, last);
    last = !last;
}

void databurn(){
    while(usbMIDI.read()); //Drop any incoming data
}
