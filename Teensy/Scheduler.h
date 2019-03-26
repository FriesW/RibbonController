#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <Arduino.h>

//typedef void (*func_t)();


class Metro {
    
    public:
        Metro();
        Metro(void (*)(), unsigned long);
        void period(unsigned long);
        void start();
        void stop();
        bool is_running() const;
        
    //Should not be externally accessed
    
        bool _if_check_then_run();
    
        void(*function)();
        unsigned long cycle;
        elapsedMillis delta;
        bool running;
};


class ManagerClass {

    public:
        
        void update();
        void attach(Metro*);
    
    private:
        Metro* metros[50];
        unsigned int length = 0;

};

void ManagerClass::attach(Metro* m){
    metros[length] = m;
    length++;
}

void ManagerClass::update(){
    for(unsigned int i = 0; i < length; i++){
        if(metros[i]->_if_check_then_run()){
            return;
        }
    }
}


ManagerClass MetroManager;


Metro::Metro(){
    cycle = -1;
    running = false;
}

Metro::Metro(void(*func)(), unsigned long period){
    function = func;
    cycle = period;
    running = false;
    MetroManager.attach(this);
}

void Metro::start(){
    if(cycle+1 == 0)
        return;
    running = true;
    delta = 0;
}

void Metro::stop(){
    running = false;
}

bool Metro::is_running() const{
    return running;
}

void Metro::period(unsigned long new_p){
    if(cycle+1 == 0)
        return;
    cycle = new_p;
}

bool Metro::_if_check_then_run(){
    if(!running)
        return false;
    unsigned long delta_static = (unsigned long) delta;
    unsigned int events_passed = delta_static / cycle;
    if(!events_passed)
        return false;
    (*function)();
    delta -= cycle * events_passed;
    return true;
}


#endif //SCHEDULER_H