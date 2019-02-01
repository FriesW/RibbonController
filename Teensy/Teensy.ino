//Teensy LC

#include <ADC.h>
#include "Settings.h"
#include "Scheduler/Scheduler.h"
#include "ControlChannel.h"
//#include "FIFO.h"

#define uint unsigned int
#define ulong unsigned long


#define led 13

#define cycle_time 5 //ms

/*** MIDI control settings ***/

//Device channel [1, 16]
const byte midi_ch = 1;

/*** MIDI notedata settings ***/
//NOTE: ADC settings are on `adc_actual_res` bits, not sampling bits

//Create linear map between adc min and max values
//and midi min and max values
//ADC ranges: [0, max `adc_actual_res` value]
//Note ranges: [0, 127]
#define adc_min 200
#define adc_max 4096 - 300 //12 bit max - 300
#define note_min 24
#define note_max 95

//Offset of low end relative to high end, added before inversion
//Use to unify signals when only one finger is touching
//Think "finger width"
#define l_to_h_offset 100

//When `note off` is sent
//ADC readings above this will assume ribbon is open circuit
#define release_cutoff adc_max + 100
//Velocity sent with each `note on`, [0, 127]
#define midi_velocity 127



/*** ADC settings ***/

//The device pins on which the ribbon ends are connected
const byte pin_l = A2;
#define pin_h A3

//Bits & averaging
#define adc_sample_res 12 //max 16
#define adc_actual_res 12 //max 12
#define adc_avg 16

//Speeds
//ADC_SAMPLING_SPEED enum:
//VERY_LOW_SPEED, LOW_SPEED, MED_SPEED, HIGH_SPEED, VERY_HIGH_SPEED
#define adc_speed_sample ADC_SAMPLING_SPEED::VERY_LOW_SPEED
//ADC_CONVERSION_SPEED enum:
//VERY_LOW_SPEED, LOW_SPEED, MED_SPEED, HIGH_SPEED_16BITS, HIGH_SPEED, VERY_HIGH_SPEED
#define adc_speed_convert ADC_CONVERSION_SPEED::VERY_LOW_SPEED



ADC adc;

#include "Ribbon.h"

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
