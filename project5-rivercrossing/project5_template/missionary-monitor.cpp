#include <iostream>
#include "ThreadClass.h"
#include "missionary-monitor.h"

MissionaryMonitor::MissionaryMonitor(char *Name)
                : Monitor(Name, HOARE), Queue("Queue")
{
    numberOfCannibal= 0;
    capCannibal = 5;
}

void MissionaryMonitor::MissionaryArrives(int mNum)
{
    MonitorBegin();
        char buff[250];
        sprintf(buff, "Missionary %d arrives\n", mNum);
        write(1, buff, strlen(buff));
        Queue.Wait();
        /*
        if (numberOfCannibal == 5)  // buffer is full
           
        numberOfCannibal++;*/
    MonitorEnd();
}