#ifndef EEPROM_RESERVE_H
#define EEPROM_RESERVE_H

#include<Arduino.h>
#include<EEPROM.h>

class EEPROMReserveClass{

    private:
        int addr_pointer = 0;
    
    public:
        int reserve(unsigned int size){
            if(addr_pointer + size > EEPROM.length())
                return -1;
            int out = addr_pointer;
            addr_pointer += size;
            return out;
        }

};

EEPROMReserveClass EEPROMReserve;

#endif //EEPROM_RESERVE_H