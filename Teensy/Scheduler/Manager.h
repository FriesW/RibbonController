#ifndef MANAGER_H
#define MANAGER_H

#include<Arduino.h>

class ManagerClass {

    public:
        
        bool update();
    
    //Should not be externally accessed
        Metro* current = NULL;

};

#endif //MANAGER_H