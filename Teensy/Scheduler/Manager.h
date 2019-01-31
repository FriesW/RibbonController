#ifndef MANAGER_H
#define MANAGER_H

#include<Arduino.h>

class ManagerClass {

    public:
        
        bool update();
    
    //Should not be externally accessed
        Metro* first;
    
    private:
        Metro* current;

};

#endif //MANAGER_H