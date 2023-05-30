
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
    void ThreeCannibals(int c1, int c2, int c3);
    void ThreeMissionaries(int m1, int m2, int m3);
    void OneCannibalsTwoMissonaries(int cNum, int m1, int m2);
    void CrossesMade(int num);
    
    int Increment();
    int Decrement(); // Should need this

    void BoatReady();   
    void BoatDone(int num, string p1, string p2, string p3);
   
    
    void SelectThreePassenger();

    void MissionaryArrives(int mNum, int mlimit);
    void CannibalArrives(int cNum, int climit);


private:
    Condition BoatQueue;
    Condition CannibalQueue;
    Condition MissionaryQueue;
    Condition LoadQueue;

    int counter;
    bool isEmpty;

    int arrivedCannibals;
    int arrivedMissionaries;
};