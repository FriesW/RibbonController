#ifndef MIDI_CONTROL_CHANNEL_H
#define MIDI_CONTROL_CHANNEL_H

#include <Arduino.h>
//#include <stdint.h>

#define CC_MODE_HIGH_RES 1
#define CC_MODE_LOW_RES 0

//http://www.somascape.org/midi/tech/spec.html#ctrlnums
#define CC_BANK_SELECT       0
#define CC_MOD_WHEEL         1
#define CC_BREATH_CTRL       2
#define CC_FOOT_CTRL         4
#define CC_PORTAMENTO_TIME   5
#define CC_DATA_ENTRY        6
#define CC_CHANNEL_VOL       7
#define CC_BALANCE           8
#define CC_PAN              10
#define CC_EXPRESSION_CTRL  11
#define CC_EFFECT_CTRL_1    12
#define CC_EFFECT_CTRL_2    13
#define CC_GEN_REG_1        16
#define CC_GEN_REG_2        17
#define CC_GEN_REG_3        18
#define CC_GEN_REG_4        19

class ControlChannel {

    public:
    
        ControlChannel(byte channel, byte msb);
        ControlChannel(byte channel, byte msb, byte resolution);
        
        int send(unsigned int value);
        
    private:
        
        byte ch;
        byte mode;
        byte reg_msb;
        byte reg_lsb;
        
        int last_msb = -1;
        int last_lsb = -1;

};

#endif //MIDI_CONTROL_CHANNEL_H