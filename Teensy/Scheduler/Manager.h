#ifndef MANAGER_H
#define MANAGER_H

#include<Arduino.h>

class ManagerClass {

    public:
        
        void update();
        void attach(Metro*);
    
    private:
        Metro* metros[50];
        unsigned int length = 0;

};

#endif //MANAGER_H