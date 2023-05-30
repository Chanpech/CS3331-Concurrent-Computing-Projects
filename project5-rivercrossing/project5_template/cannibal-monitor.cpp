#include <iostream>
#include "ThreadClass.h"
#include "cannibal-monitor.h"

CannibalMonitor::CannibalMonitor(char *Name)
                : Monitor(Name, HOARE), Queue("Queue")
{
    numberOfCannibal = 0;
    capCannibal = 4;
}

void CannibalMonitor::CannibalArrives(int cNum)
{
    char buff[250];
    sprintf(buff, "Cannibal %d Arrives\n", cNum);
    write(1, buff, strlen(buff));
    capCannibal++;
}