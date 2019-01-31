#ifndef METRO_H
#define METRO_H

#include<Arduino.h>

typedef void (*func_t)();//(int,int);
class Metro {
    
    public:
        Metro(func_t, unsigned long);
        void period(unsigned long);
        void start();
        void stop();
        
    //Should not be externally accessed
    
        bool _if_check_then_run();
    
        func_t function;
        Metro* next;
        unsigned long cycle;
        elapsedMillis delta;
        bool running;
};

#endif //METRO_H