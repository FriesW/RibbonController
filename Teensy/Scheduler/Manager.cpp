#include "Manager.h"

bool ManagerClass::update(){
    if(first == NULL)
        return false;
    if(current == NULL)
        current = first;
    
    Metro* last_attempt = current;
    
    do{
        //Move one forward
        current = current->next;
        if(current == NULL)
            current = first;
        
        //Check runstate
        if(current->_if_check_then_run())
            return true;
        
    }while(current != last_attempt);
    
    return false;
}