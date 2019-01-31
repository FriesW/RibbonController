#ifndef MIDI_CONTROL_CHANNEL_H
#define MIDI_CONTROL_CHANNEL_H

#include <Arduino.h>

#define CC_MODE_HIGH_RES (byte) 1
#define CC_MODE_LOW_RES  (byte) 0

//http://www.somascape.org/midi/tech/spec.html#ctrlnums
#define CC_BANK_SELECT      (byte)  0
#define CC_MOD_WHEEL        (byte)  1
#define CC_BREATH_CTRL      (byte)  2
#define CC_FOOT_CTRL        (byte)  4
#define CC_PORTAMENTO_TIME  (byte)  5
#define CC_DATA_ENTRY       (byte)  6
#define CC_CHANNEL_VOL      (byte)  7
#define CC_BALANCE          (byte)  8
#define CC_PAN              (byte) 10
#define CC_EXPRESSION_CTRL  (byte) 11
#define CC_EFFECT_CTRL_1    (byte) 12
#define CC_EFFECT_CTRL_2    (byte) 13
#define CC_GEN_REG_1        (byte) 16
#define CC_GEN_REG_2        (byte) 17
#define CC_GEN_REG_3        (byte) 18
#define CC_GEN_REG_4        (byte) 19

class ControlChannel {

    public:
    
        ControlChannel();
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