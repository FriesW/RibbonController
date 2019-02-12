#ifndef RIBBON_CH_SEL_H
#define RIBBON_CH_SEL_H

#include<Arduino.h>
#include "Persist/Persist.h"
#include "Linearize.h"

class RibbonChSelClass {
    
    private:
        Persist<byte> invert_s;
        Persist<byte> channel_s;
    
        unsigned int sel(unsigned int a, unsigned int b){
            if(channel_s.get())
                return a;
            else
                return b;
        }
        
        unsigned int inv(unsigned int v){
            if(invert_s.get())
                return Linearize.max_value - v;
            else
                return v;
        }
    
    public:
        
        void toggle(){
            channel_s.set( !channel_s.get() );
        }
        
        void invert(){
            invert_s.set( !invert_s.get() );
        }
        
        unsigned int process(unsigned int a, unsigned int b){
            return inv(sel(a,b));
        }
        
        
};

RibbonChSelClass RibbonChSel;

#endif //RIBBON_CH_SEL_H