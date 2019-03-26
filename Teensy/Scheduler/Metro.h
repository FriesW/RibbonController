#ifndef METRO_H
#define METRO_H

#include<Arduino.h>

typedef void (*func_t)();//(int,int);
class Metro {
    
    public:
        Metro();
        Metro(func_t, unsigned long);
        void period(unsigned long);
        void start();
        void stop();
        bool is_running() const;
        
    //Should not be externally accessed
    
        bool _if_check_then_run();
    
        func_t function;
        Metro* next = NULL;
        unsigned long cycle;
        elapsedMillis delta;
        bool running;
};

#endif //METRO_H