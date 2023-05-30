#ifndef FUNC_ALREADY_INCLUDED 
#define FUNC_ALREADY_INCLUDED 

#include "ThreadClass.h"

#endif

// Monitors
class CannibalMonitor : public Monitor // Hoare type
{
public:
    CannibalMonitor(char *Name);
    void CannibalArrives(int mNum);

private:
    Condition Queue;
    int numberOfCannibal;                 // no data items yet;
    int capCannibal;
};