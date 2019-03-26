#ifndef QUANTIZER_H
#define QUANTIZER_H

#include<Arduino.h>
#include "Settings.h"
#include "Persist/Persist.h"
#include "Linearize.h"

class QuantizerClass {
    
    private:
        Persist<byte> minimum;
        Persist<byte> maximum;
        Persist<unsigned short> scale_notes; //list of booleans
        
        byte last_note = 255;
        
        byte note_dict[128];
        byte note_count = 0;
        
        void update(){
            unsigned short scale = scale_notes.get();
            byte mi = minimum.get();
            byte ma = maximum.get();
            
            byte notes[12];
            for(byte i = 0; i < 12; i++){
                notes[i] = scale & 1;
                scale = scale >> 1;
            }
            
            byte ptr = 0;
            for(byte note = 0; note < 128; note++){
                if(note >= mi && note <= ma && notes[note%12])
                    note_dict[ptr++] = note;
            }
            note_count = ptr;
            
            for(byte i = note_count; i < 128; i++){
                note_dict[i] = note_dict[note_count-1];
            }
        }
    
    public:
        
        QuantizerClass(){
            update();
        }
        
        void set_range(byte v1, byte v2){
            byte mi = min(v1, v2);
            byte ma = max(v1, v2);
            minimum.set(mi);
            maximum.set(ma);
            update();
        }
        
        void set_scale(bool scale[12]){
            unsigned short acc = 0;
            for(byte i = 0; i < 12; i++){
                acc = acc << 1;
                acc += scale[12-1-i] & 1; //iter backwards
            }
            scale_notes.set(acc);
            update();
        }
        
        void note_on(unsigned int val){
            byte index = round( (double) val / Linearize.max_value * note_count );
            byte new_note = note_dict[index];
            if(new_note == last_note)
                return;
            note_off();
            usbMIDI.sendNoteOn(new_note, 127, MIDI_CH);
            last_note = new_note;
        }
        
        void note_off(){
            if(last_note != 255)
                usbMIDI.sendNoteOff(last_note, 0, MIDI_CH);
            last_note = 255;
        }
        
        void debug_print(){
            debug_print(false);
        }
        
        void debug_print(boolean verbose){
            static const char lut[] = "C^D^EF^G^A^B";
            unsigned short scale = scale_notes.get();
            
            byte notes[12];
            for(byte i = 0; i < 12; i++){
                notes[i] = scale & 1;
                scale = scale >> 1;
            }
            
            Serial.print(F("Notes: "));
            for(byte i = 0; i < 12; i++){
                if(notes[i])
                    Serial.print(lut[i]);
                else
                    Serial.print('_');
            }
            Serial.print(F("\nMIDI Range: ["));
            Serial.print(minimum.get());
            Serial.print(F(", "));
            Serial.print(maximum.get());
            Serial.println(F("]"));
            
            if(!verbose)
                return;
            
            Serial.println(F("Note sequence LUT:"));
            for(byte i = 0 ; i < note_count; i++){
                Serial.print(note_dict[i]);
                Serial.print(' ');
            }
            Serial.println();
            
        }
    
};


QuantizerClass Quantizer;

#endif //QUANTIZER_H