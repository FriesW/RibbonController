#ifndef LINEARIZE_H
#define LINEARIZE_H

#include<Arduino.h>
#include "adc.h"
#include "Persist/Persist.h"

class LinearizeClass {
    
    private:
        Persist<unsigned int> ribbon_r; //ohms
        
        const unsigned int sample_count = 10000;
        const unsigned int const_r = 470000; //ohms, internal resistors
        
        
        double reverse_divider(unsigned int val){
            return (double)(val * const_r) / (adc.max_reading - val);
        };
    
    public:
        
        const unsigned int max_value = 10000;
        
        void calibrate(unsigned int* old_v, unsigned int* new_v){
            
            unsigned long sum = 0;
            
            pinMode(PIN_RIBBON_A, OUTPUT);
            digitalWrite(PIN_RIBBON_A, LOW);
            for(unsigned int i = 0; i < sample_count; i++)
                sum += adc.read_B();
            pinMode(PIN_RIBBON_A, INPUT);
            
            pinMode(PIN_RIBBON_B, OUTPUT);
            digitalWrite(PIN_RIBBON_B, LOW);
            for(unsigned int i = 0; i < sample_count; i++)
                sum += adc.read_A();
            pinMode(PIN_RIBBON_B, INPUT);
            
            unsigned int apparent_r = reverse_divider( sum / (2*sample_count) );
            *old_v = ribbon_r.get();
            ribbon_r.set( apparent_r );
            *new_v = ribbon_r.get();
            
        };
        
        unsigned int map(unsigned int val){
            double apparent_r = reverse_divider(val); //actual resistance between touch and meter, ohms
            return (unsigned int)( apparent_r / ribbon_r.get() * max_value ); //scale between [0, max_value]
        };
    
};


LinearizeClass Linearize;


#endif //LINEARIZE_H