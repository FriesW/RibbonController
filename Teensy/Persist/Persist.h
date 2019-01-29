#ifndef PERSIST_H
#define PERSIST_H

#include<Arduino.h>
#include<EEPROM.h>
#include<"EEPROMReserve.h">

template <class T>
class Persist {
    
    private:
        T value;
        int addr = -1;
        
        void update(T n){
            value = n;
            if(addr != -1)
                EEPROM.put(addr, value);
        }
    
    public:
        operator T get () const{return value;}
        Persist(){
            addr = EEPROMReserve.reserve(sizeof(T));
            if(addr != -1)
                EEPROM.get(addr, value);
        }
        Persist<T> & operator = (const Persist<T> &rhs) {update(rhs.value); return *this;}
        Persist<T> & operator = (T rhs) {update(rhs); return *this;}
        Persist<T> & operator -= (T rhs) {update(rhs); return *this;}
        Persist<T> & operator += (T rhs) {update(rhs); return *this;}
        Persist<T> operator -
        Persist<T> operator +
        Persist<T> operator *
        Persist<T> operator / 
        Persist<T> operator %
        //Prefix
        Persist<T> operator++ () {update(value+1); return *this;}
        Persist<T> operator-- () {update(value-1); return *this;}
        //Postfix
        Persist<T> operator++ (T) {T o = value; update(value+1); return o;}
        Persist<T> operator-- (T) {T o = value; update(value-1); return o;}
        
        
    
    
    
};

#endif //PERSIST_H