#ifndef EXPRESSION_H
#define EXPRESSION_H

#include<Arduino.h>
#include "Settings.h"
#include "ADC.h"
#include "ControlChannel.h"
#include "FIFO.h"
#include "Scheduler/Scheduler.h"
#include "Persist/Persist.h"


void _expr_metro_cb_take_reading();
void _expr_metro_cb_out();
void _expr_metro_writeout();

class ExpressionClass {
    
    private:
    
        ControlChannel cc;
    
        FIFO<uint, 8> samples;
        
        Metro m_read;
        Metro m_out;
        Metro m_writeout;
        
        Persist<unsigned int> p_minimum;
        Persist<unsigned int> p_maximum;
        
        unsigned int minimum;
        unsigned int maximum;
        
    public:
    
        void setup(){
            cc.init(MIDI_CH, CC_EXPRESSION_CTRL, CC_MODE_HIGH_RES);
            m_read.init(_expr_metro_cb_take_reading, 4);
            m_out.init(_expr_metro_cb_out, 15);
            
            m_writeout.init(_expr_metro_writeout, 1500);
            m_writeout.start();
            minimum = p_minimum.get();
            maximum = p_maximum.get();
        }
        
        void reset_range(){
            minimum = samples.average();
            maximum = samples.average();
        }
        
        void enable(){
            m_read.start();
            m_out.start();
        }
        
        void disable(){
            m_read.stop();
            m_out.stop();
        }
        
        void _take_reading(){
            unsigned int v = adc.read(PIN_EXPRESSION);
            samples.push( v );
        }
        
        void _out(){
            unsigned int v = samples.average();
            if(samples.flushed()){
            minimum = min(minimum, v);
            maximum = max(maximum, v);
            }
            cc.send( map(v, minimum, maximum, 0, 0b11111111111111) );
        }
        
        void _writeout(){
            p_minimum.set(minimum);
            p_maximum.set(maximum);
        }
    
};

ExpressionClass Expression;

void _expr_metro_cb_take_reading(){
    Expression._take_reading();
}
void _expr_metro_cb_out(){
    Expression._out();
}

void _expr_metro_writeout(){
    Expression._writeout();
}

#endif //EXPRESSION_H