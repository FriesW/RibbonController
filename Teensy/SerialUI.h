#ifndef SERIALUI_H
#define SERIALUI_H

#include<Arduino.h>
#include "Linearize.h"


class SerialUIClass{
    
    private:
        
        boolean last_dtr;
    
        
        void drop(){
            while(-1 != Serial.read());
        }
        
        void opt_1(){
            unsigned int old_v;
            unsigned int new_v;
            Serial.println(F("Don't touch ribbon during calibration."));
            delay(1000);
            Serial.print(F("Calibrating... "));
            
            Linearize.calibrate(&old_v, &new_v);
            
            Serial.println(F("Done."));
            Serial.print(F("Ribbon resistance is "));
            Serial.print( new_v );
            Serial.println(F(" ohms."));
            Serial.print(F("Prior calibration was "));
            Serial.print( old_v );
            Serial.println(F(" ohms."));
        }
    
    public:
    
        void update(){
            
            if(!last_dtr && Serial.dtr()){
                drop();
                Serial.println(F("***** Ribbon Controller Settings *****"));
                Serial.println(F("1 : ribbon length calibration"));
                Serial.println(F("2 : quantized scale selection"));
                Serial.println(F("3 : quantized scale range"));
            }
            last_dtr = Serial.dtr();
            
            if(Serial.available() < 2)
                return;
            
            char c = Serial.read();
            char n = Serial.peek();
            if(n != '\n' && n != '\r')
                return;
            Serial.read();
            
            bool valid = c >= '1' && c <= '3';
            
            if(valid)
                Serial.println();
            
            if(c == '1') opt_1();
            if(c == '2')
                Serial.println(F("Input new quantization scale..."));
            if(c == '3')
                Serial.println(F("Input new quantized scale range..."));
            
            if(valid){
                Serial.println();
                last_dtr = false;
            }
                    
        };
    
    
};

SerialUIClass SerialUI;


#endif //SERIALUI_H