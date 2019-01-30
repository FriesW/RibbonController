#ifndef MANAGER_H
#define MANAGER_H

#include<Arduino.h>
#include "Metro.h"

class ManagerClass {

    public:
        
        bool update();
    
    //Should not be externally accessed
        Metro* first;
    
    private:
        Metro* current;

};

ManagerClass MetroManager;

#endif //MANAGER_H