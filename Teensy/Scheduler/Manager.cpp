bool ManagerClass::update(){
    if(current == NULL)
        return false;
    
    Metro* last_attempt = current;
    
    do{
        //Move one forward
        current = current->next;
        
        //Check runstate
        if(current->_if_check_then_run())
            return true;
        
    }while(current != last_attempt);
    
    return false;
}