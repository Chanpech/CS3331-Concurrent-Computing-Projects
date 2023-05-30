// ------------------------------------------------
// NAME : Chanpech Hoeng ID: choeng
// DUE DATE : 04/19/2023
// PROGRAM ASSIGNMENT #5
// FILE NAME : boat-monitor.h 
// PURPOSE :
// Declared an interfaces for a monitor: BoatMonitor
// -------------------------------------------------

#ifndef MY_FUNCTIONS_INCLUDED
#define MY_FUNCTIONS_INCLUDED

#include "ThreadClass.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <queue>
#endif

class BoatMonitor : public Monitor
{
public:
    BoatMonitor(char *Name); // initialization -- constructor
    void BoatReady();   
    void BoatDone(int num);

    void MissionaryArrives(int mNum, int mLimit, int *mArray);
    void CannibalArrives(int cNum, int cLimit, int *cArray);

private:
    Condition BoatQueue;
    Condition CannibalQueue;
    Condition MissionaryQueue;
    Condition LoadQueue;

    int counter;
    bool isEmpty;
    int cannibalsLimit;
    int missionariesLimit;
    int arrivedCannibals;
    int arrivedMissionaries;
    int *cannibalsArray;
    int *missionariesArray;
    int cannibalReturned;
    int missionaryReturned;


    queue<int> cannibalQueue;
    queue<int> missionaryQueue;
    queue<int> passengerQueue;

    bool threeCannibalsSelected;
    bool threeMissionariesSelected;
    bool mixedSelection;

    int findAvailableCannibal();
    int findAvailableMissionary();
    
    void ThreeCannibals();
    void ThreeMissionaries();
    void OneCannibalsTwoMissonaries();
    void CrossesMade(int num);
    void SelectThreePassenger();
    void BoatLoadComplete();
    
    void SetCannibalsLimit(int climit);
    void SetMissionariesLimit(int mlimit);
};