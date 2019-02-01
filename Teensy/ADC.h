#ifndef ADC_LOCAL_H
#define ADC_LOCAL_H

#include "Settings.h"
#include<ADC.h>


class AdcWithSetup : public ADC {
    
    public:
        AdcWithSetup(){
            setResolution(ADC_SAMPLE_RES);
            setAveraging(ADC_AVERAGES);
            setSamplingSpeed(ADC_SAMPLE_SPEED);
            setConversionSpeed(ADC_CONVERT_SPEED);
        }
        
        unsigned int read_A(){
            return analogRead(PIN_RIBBON_A) >> (ADC_SAMPLE_RES - ADC_ACTUAL_RES);
        };
        unsigned int read_B(){
            return analogRead(PIN_RIBBON_B) >> (ADC_SAMPLE_RES - ADC_ACTUAL_RES);
        };
    
};


AdcWithSetup adc;

#endif