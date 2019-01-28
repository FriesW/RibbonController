//Teensy LC

#include <ADC.h>
#define uint unsigned int
#define ulong unsigned long


#define led 13

#define cycle_time 5 //ms

/*** MIDI control settings ***/

//Device channel [1, 16]
#define midi_ch 1

//High res continuous controller addresses
//for reporting h and l positions
//Format: `msb, lsb`
//See: http://www.somascape.org/midi/tech/spec.html#ctrlnums
#define reg_l 16, 48
#define reg_h 17, 49


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



ADC *adc = new ADC();

void setup(){
    pinMode(led, OUTPUT);
    
    adc->setResolution(adc_sample_res);
    adc->setAveraging(adc_avg);
    adc->setSamplingSpeed(adc_speed_sample);
    adc->setConversionSpeed(adc_speed_convert);
}

void loop(){
    ratelimit();
    databurn();
    alive();
    
    uint low;
    uint high;
    
    read_ribbon(&low, &high);
    
    note_out(low, high);
    
}

void read_ribbon(uint* out_l, uint* out_h){
    static uint last_l = -1;
    static uint last_h = -1;
    
    ulong read_l = 0;
    ulong read_h = 0;
    for(uint i = 0; i < logic_avg; i++){
        read_l += adc->analogRead(pin_l);
        read_h += adc->analogRead(pin_h);
    }
    uint new_l = read_l / logic_avg;
    uint new_h = read_h / logic_avg;
    
    new_l = new_l >> (adc_sample_res - adc_actual_res);
    new_h = new_h >> (adc_sample_res - adc_actual_res);
    dbp(new_l, new_h);
    
    midi_control_out(reg_l, new_l, last_l);
    midi_control_out(reg_h, new_h, last_h);
    
    *out_l = last_l = new_l;
    *out_h = last_h = new_h;
}

void midi_control_out(uint msb_r, uint lsb_r, uint new_v, uint old_v){
    byte msb_nd = new_v >> 7;
    byte lsb_nd = new_v & 0b1111111;
    byte msb_od = old_v >> 7;
    byte lsb_od = old_v & 0b1111111;
    boolean send_msb = msb_nd != msb_od;
    boolean send_lsb = send_msb || lsb_nd != lsb_od;
    if(send_msb)
        usbMIDI.sendControlChange(msb_r, msb_nd, midi_ch);
    if(send_lsb)
        usbMIDI.sendControlChange(lsb_r, lsb_nd, midi_ch);
}

void note_out(uint low, uint high){
    static uint[] buffer;
    static elapsedMillis hold_off;
    static boolean state = false;
    
    if(state){
    }
    else{
        if(high > 2900){ //TODO -> constant
            hold_off = 0;
            return;
        }
        if(hold_off > 20){ //TODO -> constant
            state = false;
        }
    }
}

void ratelimit(){
    static elapsedMillis t;
    while(t < cycle_time) usbMIDI.read();
    t -= cycle_time;
}

void alive(){
    static elapsedMillis t;
    static boolean last = false;
    if(t > 500){
        digitalWrite(led, last);
        last = !last;
        t -= 500;
    }
}

void databurn(){
    while(usbMIDI.read()); //Drop any incoming data
}

void dbp(uint a, uint b){
    dbp_ws(a);
    Serial.print(a);
    Serial.print("   ");
    dbp_ws(b);
    Serial.println(b);
}

void dbp_ws(uint a){
    #define pws )Serial.print(' ');if(a<
    if(a <
    10L pws
    100L pws
    1000L pws
    10000L pws
    100000L pws
    1000000L pws
    10000000L pws
    -1);
}