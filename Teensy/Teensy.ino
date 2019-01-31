//Teensy LC

#include <ADC.h>
#include "Settings.h"
#include "Scheduler/Scheduler.h"
#include "ControlChannel.h"

#define uint unsigned int
#define ulong unsigned long


#define led 13

#define cycle_time 5 //ms

/*** MIDI control settings ***/

//Device channel [1, 16]
#define midi_ch 1

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
#define pin_l A2
#define pin_h A3

//Bits & averaging
#define adc_sample_res 16 //max 16
#define adc_actual_res 12 //max 12
#define adc_avg 32
#define logic_avg 10

//Speeds
//ADC_SAMPLING_SPEED enum:
//VERY_LOW_SPEED, LOW_SPEED, MED_SPEED, HIGH_SPEED, VERY_HIGH_SPEED
#define adc_speed_sample ADC_SAMPLING_SPEED::VERY_LOW_SPEED
//ADC_CONVERSION_SPEED enum:
//VERY_LOW_SPEED, LOW_SPEED, MED_SPEED, HIGH_SPEED_16BITS, HIGH_SPEED, VERY_HIGH_SPEED
#define adc_speed_convert ADC_CONVERSION_SPEED::VERY_LOW_SPEED



ADC adc;

ControlChannel pt_l (midi_ch, CC_GEN_REG_1, CC_MODE_HIGH_RES);
ControlChannel pt_h (midi_ch, CC_GEN_REG_2, CC_MODE_HIGH_RES);
ControlChannel expr (midi_ch, CC_EXPRESSION_CTRL, CC_MODE_HIGH_RES);

Metro reader (make_reading, 20);
Metro heart_beat (alive, 500);

void setup(){
    Serial.begin(0);
    Serial.println(settings.x.get());
    Serial.println(settings.y.get());
    Serial.println(settings.z.get());
    
    pinMode(led, OUTPUT);
    
    adc.setResolution(adc_sample_res);
    adc.setAveraging(adc_avg);
    adc.setSamplingSpeed(adc_speed_sample);
    adc.setConversionSpeed(adc_speed_convert);
    reader.start();
    heart_beat.start();
}

void loop(){
    databurn();
    MetroManager.update();
}

void make_reading(){
    uint low;
    uint high;
    
    read_ribbon(&low, &high);
    
    pt_l.send(low);
    pt_h.send(high);
}

void read_ribbon(uint* out_l, uint* out_h){
    ulong read_l = 0;
    ulong read_h = 0;
    for(uint i = 0; i < logic_avg; i++){
        read_l += adc.analogRead(pin_l);
        read_h += adc.analogRead(pin_h);
    }
    uint new_l = read_l / logic_avg;
    uint new_h = read_h / logic_avg;
    
    new_l = new_l >> (adc_sample_res - adc_actual_res);
    new_h = new_h >> (adc_sample_res - adc_actual_res);
    
    *out_l = new_l;
    *out_h = new_h;
}

void alive(){
    static boolean last = false;
    digitalWrite(led, last);
    last = !last;
}

void databurn(){
    while(usbMIDI.read()); //Drop any incoming data
}
