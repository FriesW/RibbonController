#ifndef SCHEDULER_H
#define SCHEDULER_H

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

class ManagerClass {

    public:
        
        bool update();
    
    //Should not be externally accessed
        Metro* first;
    
    private:
        Metro* current;

};

//ManagerClass MetroManager;

#endif //SCHEDULER_H