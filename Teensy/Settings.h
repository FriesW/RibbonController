#ifndef SETTINGS_H
#define SETTINGS_H

#include<Arduino.h>
#include "Persist/Persist.h"

struct {

    Persist<int> x;
    Persist<int> y;
    Persist<unsigned long> z;

} settings;

#endif //SETTINGS_H