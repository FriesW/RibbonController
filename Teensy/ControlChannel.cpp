#include "ControlChannel.h"

ControlChannel::ControlChannel(byte channel, byte msb){
    ch = channel;
    mode = CC_MODE_LOW_RES;
    reg_msb = msb;
}

ControlChannel::ControlChannel(byte channel, byte msb, byte resolution){
    ch = channel;
    mode = resolution;
    reg_msb = msb;
    reg_lsb = msb + 32;
}

int ControlChannel::send(unsigned int value){
    
    if(mode == CC_MODE_LOW_RES){
        byte out = value & 0b1111111;
        if(out != last_msb){
            last_msb = out;
            usbMIDI.sendControlChange(reg_msb, out, ch);
            return 1;
        }
        return 0;
    }
    
    else if(mode == CC_MODE_HIGH_RES){
        byte out_msb = (value >> 7) & 0b1111111;
        byte out_lsb = value & 0b1111111;
        byte send_msb = out_msb != last_msb;
        byte send_lsb = out_lsb != last_lsb || send_msb;
        last_msb = out_msb;
        last_lsb = out_lsb;
        if(send_msb)
            usbMIDI.sendControlChange(reg_msb, out_msb, ch);
        if(send_lsb)
            usbMIDI.sendControlChange(reg_lsb, out_lsb, ch);
        return send_lsb;
        
    }
    
    return -1;
    
}