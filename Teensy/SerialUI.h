#ifndef SERIALUI_H
#define SERIALUI_H

#include<Arduino.h>
#include "Linearize.h"
#include "Quantizer.h"
#include "RibbonChSel.h"
#include "Expression.h"

class SerialUIClass{
    
    private:
        
        boolean last_dtr;
    
        
        void drop(){
            while(-1 != Serial.read());
        }
        
        void wait(){
            while(!Serial.available());
        }
        
        unsigned int parseInt(){
            wait();
            while(Serial.peek() < '0' || Serial.peek() > '9'){
                Serial.read();
                wait();
            }
            unsigned int acc = 0;
            while(Serial.peek() >= '0' && Serial.peek() <= '9'){
                acc = (acc*10) + (Serial.read() - '0');
                wait();
            }
            return acc;
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
        
        void opt_2(){
            Expression.reset_range();
            Serial.println(F("Expression lever range has been reset."));
            Serial.println(F("Move expression lever through entire movement range."));
        }
        
        void opt_3(){
            Serial.println(F("Input new quantization scale."));
            Serial.println(F("Input 12 characters, representing the twelve chromatic scale notes, starting at 'C'."));
            Serial.println(F(" In: [a-z|A-Z|#^] -> A letter or '#' or '^' will include that note's position in the scale."));
            Serial.println(F("Out: [_-*,./\\?' ] -> Punctuation or a space will exclude that note's position in the scale."));
            Serial.println(F("Template chromatic scale: C^D^EF^G^A^B"));
            Serial.println(F("        Template C scale: C_D_EF_G_A_B"));
            Serial.println(F("Input new scale:"));
            
            bool notes[12];
            for(byte i = 0; i < 12; i++){
                wait();
                byte v = Serial.read();
                if( (v>='a' && v<='z') || (v>='A' && v<='Z') || v=='#' || v=='^' )
                    notes[i] = true;
                else if ( v=='_' || v=='-' || v=='*' || v==',' || v=='.' || v=='/' || v=='\\' || v=='?' || v=='\'' || v==' ' )
                    notes[i] = false;
                else if (v=='r' || v=='\n'){
                    i--;
                }
                else{
                    Serial.print(F("Invalid: input character '"));
                    Serial.print((char)v);
                    Serial.print(F("' at position "));
                    Serial.print(i);
                    Serial.println(F(" not recognized."));
                    return;
                }
            }
            
            bool valid = false;
            for(byte i = 0; i < 12; i++)
                valid = valid || notes[i];
            if(!valid){
                Serial.println("Invalid: At least one note must be turned on.");
                return;
            }
            
            Quantizer.set_scale(notes);
            
            Quantizer.debug_print();
        }
        
        void opt_4(){
            Serial.println(F("Input new quantized scale range."));
            Serial.println(F("Input two midi note numbers [0,127]:"));
            
            unsigned int n1 = parseInt();
            if(n1 > 127){
                Serial.println(F("Invalid: number out of range."));
                return;
            }
            unsigned int n2 = parseInt();
            if(n2 > 127){
                Serial.println(F("Invalid: number out of range."));
                return;
            }
            
            if(n1 == n2){
                Serial.println(F("Invalid: numbers must define a range."));
                return;
            }
            
            unsigned int mi = min(n1, n2);
            unsigned int ma = max(n1, n2);
            
            Quantizer.set_range(mi, ma);
            
            Quantizer.debug_print();
        }
        
        void opt_5(){
            RibbonChSel.invert();
            Serial.println(F("Inverted: quantized output"));
        }
        
        void opt_6(){
            RibbonChSel.toggle();
            Serial.println(F("Toggled: ribbon end for quantized output"));
        }
        
        void opt_7(){
            Quantizer.debug_print(true);
        }
    
    public:
    
        void update(){
            
            if(!last_dtr && Serial.dtr()){
                drop();
                Serial.println(F("***** Ribbon Controller Settings *****"));
                Serial.println(F("1 : calibrate ribbon length/resistance"));
                Serial.println(F("2 : calibrate expression lever"));
                Serial.println(F("3 : set quantized scale selection"));
                Serial.println(F("4 : set quantized scale range"));
                Serial.println(F("5 : invert quantized output"));
                Serial.println(F("6 : toggle ribbon end for quantized output"));
                Serial.println(F("7 : print quantizer status"));
                Serial.print(F("\r\n\r\n"));
            }
            last_dtr = Serial.dtr();
            
            if(!Serial.available())
                return;
            char c = Serial.read();
            if(c < '1' || c > '7'){
                last_dtr = false;
                return;
            }
            
            delay(5); //wait for all incomming chars
            drop();
            
            if(c == '1') opt_1();
            if(c == '2') opt_2();
            if(c == '3') opt_3();
            if(c == '4') opt_4();
            if(c == '5') opt_5();
            if(c == '6') opt_6();
            if(c == '7') opt_7();
            
            Serial.println();
            drop();
        };
    
    
};

SerialUIClass SerialUI;


#endif //SERIALUI_H