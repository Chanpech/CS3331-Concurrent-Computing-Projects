
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
    int findAvailableCannibal();
    void ThreeCannibals();
    void ThreeMissionaries();
    void OneCannibalsTwoMissonaries();

    
    int Increment();
    int Decrement(); // Should need this


    void SelectThreePassenger();

    void BoatReady();
    void BoatDone(int num, string p1, string p2, string p3);
    //void BoatLoad(int p1, int p2, int p3);

    void MissionaryArrives(int mNum, int mlimit, int *missionariesArray);
    void CannibalArrives(int cNum, int climit, int *cannibalArray);

private:
    Condition LoadQueue;
    Condition BoatQueue;
    Condition CannibalQueue;
    Condition MissionaryQueue;

    int Limit; // Boat counter, will used for conditional comparison
    int counter;
    bool isEmpty;
    
    int arrivedCannibals;
    int arrivedMissionaries;

    int *cannibalsArray;
    int *missionariesArray;
    
    //string threePassenger[3];
};