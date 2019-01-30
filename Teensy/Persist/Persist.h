#ifndef PERSIST_H
#define PERSIST_H

#include<Arduino.h>
#include<EEPROM.h>
#include "EEPROMReserve.h"

template <class T>
class Persist {
    
    private:
        T value;
        int addr = -1;
    
    public:
    
        Persist(){
            addr = EEPROMReserve.reserve(sizeof(T));
            if(working())
                EEPROM.get(addr, value);
        }
        
        T get(){
            return value;
        }
        
        void set(T n){
            value = n;
            if(working())
                EEPROM.put(addr, value);
        }
        
        bool working(){
            return addr != -1;
        }

};

#endif //PERSIST_H