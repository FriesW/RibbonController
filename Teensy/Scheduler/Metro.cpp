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
    unsigned long delta_static = (unsigned long) delta;
    unsigned int events_passed = delta_static / cycle;
    if(!events_passed)
        return false;
    function();
    delta -= cycle * events_passed;
    return true;
}