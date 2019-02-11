//Teensy LC

#include "ADC.h"
#include "Settings.h"
#include "Scheduler/Scheduler.h"
#include "Ribbon.h"
#include "Linearize.h"

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
    
    while(!Serial.dtr());
    Serial.println("Calibrating...");
    Linearize.calibrate();
    Serial.println("Done");
}

void loop(){
    databurn();
    //serial_ui();
    MetroManager.update();
}


void serial_ui(){
    static boolean last_dtr;
    
    if(!last_dtr && Serial.dtr()){
        drop();
        Serial.println(F("***** Ribbon Controller Settings *****"));
        Serial.println(F("1 : ribbon length calibration"));
        Serial.println(F("2 : quantized scale selection"));
        Serial.println(F("3 : quantized scale range"));
    }
    last_dtr = Serial.dtr();
    
    if(Serial.available() < 2)
        return;
    
    char c = Serial.read();
    char n = Serial.peek();
    if(n != '\n' && n != '\r')
        return;
    Serial.read();
    
    if(c == '1')
        Serial.println(F("Calibrating ribbon length..."));
    if(c == '2')
        Serial.println(F("Input new quantization scale..."));
    if(c == '3')
        Serial.println(F("Input new quantized scale range..."));
    
    if(c >= '1' && c <= '3'){
        Serial.println();
        last_dtr = false;
    }
    
}

void drop(){
    while(-1 != Serial.read());
}

void alive(){
    static boolean last = false;
    digitalWrite(led, last);
    last = !last;
}

void databurn(){
    while(usbMIDI.read()); //Drop any incoming data
}
