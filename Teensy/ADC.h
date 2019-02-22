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
        
        const unsigned int max_reading = (1 << ADC_ACTUAL_RES) - 1;
        
        unsigned int read(unsigned int pin){
            return analogRead(pin) >> (ADC_SAMPLE_RES - ADC_ACTUAL_RES);
        }
    
};


AdcWithSetup adc;

#endif