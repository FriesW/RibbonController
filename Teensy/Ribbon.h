#ifndef RIBBON_H
#define RIBBON_H

#include<Arduino.h>
#include "Settings.h"
#include "ADC.h"
#include "ControlChannel.h"
#include "FIFO.h"
#include "Scheduler/Scheduler.h"
#include "Linearize.h"
#include "RibbonChSel.h"
#include "Quantizer.h"



void _rib_metro_cb_take_reading();
void _rib_metro_cb_out_flow();
void _rib_metro_cb_out_raw();

struct _RibbonChannel {
    ControlChannel cc_raw;
    ControlChannel cc_flow;
    FIFO<uint, 5> ff_tail;
    FIFO<uint, 5> ff_sample;
    FIFO<uint, 5> ff_head;
};


class RibbonClass {
    
    private:
    
        ControlChannel state;
    
        struct _RibbonChannel a;
        struct _RibbonChannel b;
    
        Metro m_read;
        Metro m_flow;
        Metro m_raw;
    
        void sound_off(){
            state.send(0);
            Quantizer.note_off();
        }
    
    
    
    public:
    
        void setup(){
            a.cc_flow.init(MIDI_CH, CC_GEN_REG_1, CC_MODE_HIGH_RES);
            b.cc_flow.init(MIDI_CH, CC_GEN_REG_2, CC_MODE_HIGH_RES);
            a.cc_raw.init(MIDI_CH, CC_GEN_REG_3, CC_MODE_HIGH_RES);
            b.cc_raw.init(MIDI_CH, CC_GEN_REG_4, CC_MODE_HIGH_RES);
            state.init(MIDI_CH, CC_CHANNEL_VOL, CC_MODE_LOW_RES);
            m_read.init(_rib_metro_cb_take_reading, 4);
            m_flow.init(_rib_metro_cb_out_flow, 15);
            m_raw.init(_rib_metro_cb_out_raw, 15);
        }
        
        
        void enable(){
            m_read.start();
            m_raw.start();
        }
        
        
        void disable(){
            m_read.stop();
            m_flow.stop();
            m_raw.stop();
            sound_off();
        }
        
        
        void _take_reading(){
            unsigned int va = adc.read(PIN_RIBBON_A);
            unsigned int vb = adc.read(PIN_RIBBON_B);
            a.ff_tail.push(
              a.ff_sample.push(
                a.ff_head.push( va )
            ));
            b.ff_tail.push(
              b.ff_sample.push(
                b.ff_head.push( vb )
            ));
            
            bool thres =
                a.ff_head.max() > 3200 ||
                b.ff_head.max() > 3200 ||
                a.ff_tail.max() > 3200 ||
                b.ff_tail.max() > 3200 ||
                a.ff_sample.max() > 3200 ||
                b.ff_sample.max() > 3200;
            bool running = m_flow.is_running();
            
            if(!running && !thres){
                m_flow.start();
                _out_flow();
                state.send(127); //_sound_on
            }
            if(running && thres){
                m_flow.stop();
                _out_flow();
                sound_off();
            }
        }

        void _out_flow(){
            unsigned int va = Linearize.map( a.ff_sample.average() );
            unsigned int vb = Linearize.max_value - Linearize.map( b.ff_sample.average() );
            Quantizer.note_on( RibbonChSel.process(va, vb) );
            a.cc_flow.send(va);
            b.cc_flow.send(vb);
        }

        void _out_raw(){
            a.cc_raw.send( a.ff_head.average() );
            b.cc_raw.send( b.ff_head.average() );
        }
        
        
    
};

RibbonClass Ribbon;


void _rib_metro_cb_take_reading(){
    Ribbon._take_reading();
}
void _rib_metro_cb_out_flow(){
    Ribbon._out_flow();
}
void _rib_metro_cb_out_raw(){
    Ribbon._out_raw();
}

#endif //RIBBON_H