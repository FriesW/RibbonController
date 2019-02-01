#ifndef SETTINGS_H
#define SETTINGS_H


/*** MIDI control settings ***/

//Device channel [1, 16]
#define MIDI_CH 1


/*** ADC settings ***/

//The device pins on which the ribbon ends are connected
#define PIN_RIBBON_A   A2
#define PIN_RIBBON_B   A3

//Bits & averaging
#define ADC_SAMPLE_RES 12 //max 16
#define ADC_ACTUAL_RES 12 //max 12
#define ADC_AVERAGES 16 //power of 2

//Speeds
//ADC_SAMPLING_SPEED enum:
//VERY_LOW_SPEED, LOW_SPEED, MED_SPEED, HIGH_SPEED, VERY_HIGH_SPEED
#define ADC_SAMPLE_SPEED  ADC_SAMPLING_SPEED::VERY_LOW_SPEED
//ADC_CONVERSION_SPEED enum:
//VERY_LOW_SPEED, LOW_SPEED, MED_SPEED, HIGH_SPEED_16BITS, HIGH_SPEED, VERY_HIGH_SPEED
#define ADC_CONVERT_SPEED  ADC_CONVERSION_SPEED::VERY_LOW_SPEED



#endif //SETTINGS_H