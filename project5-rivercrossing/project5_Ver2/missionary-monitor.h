#ifndef FUNC_ALREADY_INCLUDED
#define FUNC_ALREADY_INCLUDED

#include "ThreadClass.h"
#endif

// Monitors
class MissionaryMonitor : public Monitor // Hoare type
{
    public:
        MissionaryMonitor(char *Name);
        void MissionaryArrives(int mNum);

    private:
        Condition Queue;
        int numberOfCannibal;                 // no data items yet;
        int capCannibal;
};
