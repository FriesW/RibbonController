#ifndef EXPRESSION_H
#define EXPRESSION_H

#include<Arduino.h>
#include "Settings.h"
#include "ADC.h"
#include "ControlChannel.h"
#include "FIFO.h"
#include "Scheduler/Scheduler.h"



void _expr_metro_cb_take_reading();
void _expr_metro_cb_out();

class ExpressionClass {
    
    private:
    
        ControlChannel cc;
    
        FIFO<uint, 5> samples;
        
        Metro m_read;
        Metro m_out;
        
    public:
    
        void setup(){
            cc = ControlChannel(MIDI_CH, CC_EXPRESSION_CTRL, CC_MODE_HIGH_RES);
            m_read = Metro(_expr_metro_cb_take_reading, 4);
            m_out = Metro(_expr_metro_cb_out, 15);
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
            cc.send( samples.average() );
        }
    
};

ExpressionClass Expression;

void _expr_metro_cb_take_reading(){
    Expression._take_reading();
};
void _expr_metro_cb_out(){
    Expression._out();
};

#endif //EXPRESSION_H