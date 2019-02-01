#ifndef RIBBON_H
#define RIBBON_H

#include<Arduino.h>
#include "Settings.h"
#include "ADC.h"
#include "ControlChannel.h"
#include "FIFO.h"
#include "Scheduler/Scheduler.h"


struct RibbonChannel {
    ControlChannel cc_raw;
    ControlChannel cc_flow;
    FIFO<uint, 5> ff_tail;
    FIFO<uint, 5> ff_sample;
    FIFO<uint, 5> ff_head;
};

typedef void (*func_t)();
struct StaticRibbon {
    func_t setup;
    func_t enable;
    func_t disable;
};

void _ribbon_init();
struct StaticRibbon Ribbon = {.setup = _ribbon_init};


struct RibbonChannel _a;
struct RibbonChannel _b;

void _enable();
void _disable();

void _take_reading();
void _out_flow();
void _out_raw();

Metro _m_read (_take_reading, 4);
Metro _m_flow (_out_flow, 15);
Metro _m_raw (_out_raw, 15);



void _ribbon_init(){
    _a.cc_flow = ControlChannel(MIDI_CH, CC_GEN_REG_1, CC_MODE_HIGH_RES);
    _b.cc_flow = ControlChannel(MIDI_CH, CC_GEN_REG_2, CC_MODE_HIGH_RES);
    _a.cc_raw  = ControlChannel(MIDI_CH, CC_GEN_REG_3, CC_MODE_HIGH_RES);
    _b.cc_raw  = ControlChannel(MIDI_CH, CC_GEN_REG_4, CC_MODE_HIGH_RES);
    Ribbon.enable = _enable;
    Ribbon.disable = _disable;
}

void _enable(){
    _m_read.start();
    _m_raw.start();
}

void _disable(){
    _m_read.stop();
    _m_flow.stop();
    _m_raw.stop();
}

void _take_reading(){
    unsigned int va = adc.read_A();
    unsigned int vb = adc.read_B();
    _a.ff_tail.push(
      _a.ff_sample.push(
        _a.ff_head.push( va )
    ));
    _b.ff_tail.push(
      _b.ff_sample.push(
        _b.ff_head.push( vb )
    ));
    
    bool thres =
        _a.ff_head.max() > 3200 ||
        _a.ff_tail.max() > 3200 ||
        _b.ff_head.max() > 3200 ||
        _b.ff_tail.max() > 3200 ||
        _a.ff_sample.max() > 3200 ||
        _b.ff_sample.max() > 3200;
    bool running = _m_flow.is_running();
    
    if(!running && !thres){
        _m_flow.start();
        _out_flow();
    }
    if(running && thres){
        _m_flow.stop();
        _out_flow();
    }
}

void _out_flow(){
    _a.cc_flow.send( _a.ff_sample.average() );
    _b.cc_flow.send( _b.ff_sample.average() );
}

void _out_raw(){
    _a.cc_raw.send( _a.ff_head.average() );
    _b.cc_raw.send( _b.ff_head.average() );
}


#endif //RIBBON_H