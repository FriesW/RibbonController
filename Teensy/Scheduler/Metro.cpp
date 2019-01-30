#include "Scheduler.h"
#include "Manager.h"

Metro::Metro(func_t func, unsigned long period){
    function = func;
    cycle = period;
    running = false;
    //Add to linked list
    //First Metro made
    if(MetroManager.first == NULL){
        MetroManager.first = this;
    }
    //Add to end of list
    else{
        Metro* last = MetroManager.first;
        while(last->next != NULL)
            last = last->next;
        last->next = this;
    }
}

void Metro::start(){
    running = true;
    delta = 0;
}

void Metro::stop(){
    running = false;
}

void Metro::period(unsigned long new_p){
    cycle = new_p;
}

bool Metro::_if_check_then_run(){
    if(!running)
        return false;
    if(delta < cycle)
        return false;
    function();
    while(delta > cycle)
        delta -= cycle;
    return true;
}