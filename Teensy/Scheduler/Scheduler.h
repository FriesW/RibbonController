//Nasty workaround, ensures build order

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Metro.h"
#include "Manager.h"

#include "Manager.cpp"

ManagerClass MetroManager;

#include "Metro.cpp"

#endif //SCHEDULER_H
