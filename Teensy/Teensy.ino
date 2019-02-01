//Teensy LC

#include "ADC.h"
#include "Settings.h"
#include "Scheduler/Scheduler.h"
//#include "ControlChannel.h"
#include "Ribbon.h"

#define uint unsigned int
#define ulong unsigned long


#define led 13


/*** MIDI control settings ***/

//Device channel [1, 16]
#define midi_ch 1


/*** ADC settings ***/

//The device pins on which the ribbon ends are connected
#define pin_l A2
#define pin_h A3

//Bits & averaging
#define adc_sample_res 12 //max 16
//#define adc_actual_res 12 //max 12
#define adc_avg 16

//Speeds
//ADC_SAMPLING_SPEED enum:
//VERY_LOW_SPEED, LOW_SPEED, MED_SPEED, HIGH_SPEED, VERY_HIGH_SPEED
#define adc_speed_sample ADC_SAMPLING_SPEED::VERY_LOW_SPEED
//ADC_CONVERSION_SPEED enum:
//VERY_LOW_SPEED, LOW_SPEED, MED_SPEED, HIGH_SPEED_16BITS, HIGH_SPEED, VERY_HIGH_SPEED
#define adc_speed_convert ADC_CONVERSION_SPEED::VERY_LOW_SPEED




//ControlChannel expr (midi_ch, CC_EXPRESSION_CTRL, CC_MODE_HIGH_RES);
Metro heart_beat (alive, 500);


void setup(){
    Serial.begin(0);
    
    pinMode(led, OUTPUT);
    
    adc.setResolution(adc_sample_res);
    adc.setAveraging(adc_avg);
    adc.setSamplingSpeed(adc_speed_sample);
    adc.setConversionSpeed(adc_speed_convert);
    
    heart_beat.start();
    Ribbon.setup(midi_ch, pin_l, pin_h);
    Ribbon.enable();
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
