Metro::Metro(){}

Metro::Metro(func_t func, unsigned long period){
    init(func, period);
}

void Metro::init(func_t func, unsigned long period){
    function = func;
    running = false;
    cycle = period;
    
    //Add to circular linked list
    if(MetroManager.current == NULL)
        MetroManager.current = this;
    next = MetroManager.current->next;
    MetroManager.current->next = this;
}

void Metro::start(){
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
    cycle = new_p;
}

bool Metro::_if_check_then_run(){
    if(function == NULL)
        return false;
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