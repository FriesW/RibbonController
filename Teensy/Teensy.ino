//Teensy LC
//Serial
//48 Mhz
//Smallest code

#define pl(X) Serial.println(X);Serial.flush()

#include "Scheduler/Scheduler.h"

#define uint unsigned int
#define ulong unsigned long


#define led 13


Metro heart_beat = Metro(alive, 500);
Metro m1 = Metro(c1, 5000);
Metro m2 = Metro(c2, 5000);
Metro m3 = Metro(c3, 5000);
//Metro* heart_beat = new Metro(alive, 500);


void setup(){
    Serial.begin(0);
    while(!Serial.dtr());
    pl("Begin");

    pinMode(led, OUTPUT);
    digitalWrite(led, HIGH);

    pl((ulong) MetroManager.first);
    pl((ulong) MetroManager.current);

    heart_beat.start();
    m1.start();
    m2.start();
    m3.start();

    pl((ulong) MetroManager.first);
    pl((ulong) MetroManager.current);

    MetroManager.update();

    pl((ulong) MetroManager.first);
    pl((ulong) MetroManager.current);

}

void loop(){
    MetroManager.update();
}

void alive(){
    static boolean last = true;
    digitalWrite(led, last);
    last = !last;
}

void c1(){Serial.println("c1");}
void c2(){Serial.println("c2");}
void c3(){Serial.println("c3");}
