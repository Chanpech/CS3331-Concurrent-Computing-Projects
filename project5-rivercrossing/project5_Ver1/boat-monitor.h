
#ifndef MY_FUNCTIONS_INCLUDED
#define MY_FUNCTIONS_INCLUDED

#include "ThreadClass.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#endif

class BoatMonitor : public Monitor
{
public:
    BoatMonitor(char *Name); // initialization -- constructor
    void ThreeCannibals(int monNum, int c1, int c2, int c3);
    void ThreeMissionaries(int monNum, int m1, int m2, int m3);
    void OneCannibalsTwoMissonaries(int monNum, int cNum, int m1, int m2);
    void CrossesMade(int num);
    int Increment();
    int Decrement(); // Should need this

    void BoatReady();
    void BoatDone(int num);

    void MissionaryArrives(int mNum);
    void CannibalArrives(int mNum);

private:
    Condition BoatQueue;
    Condition CannibalQueue;
    Condition MissionaryQueue;
    int Limit; // Boat counter, will used for conditional comparison
    int counter;
    bool isEmpty;
};