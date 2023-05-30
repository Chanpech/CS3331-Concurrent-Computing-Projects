// ------------------------------------------------
// NAME : Chanpech Hoeng ID: choeng
// DUE DATE : 04/6/2023
// PROGRAM ASSIGNMENT #5
// FILE NAME : thread.cpp
// PROGRAM PURPOSE :
// -------------------------------------------------

#include "ThreadClass.h"
#include <stdlib.h>
#include <iostream>
#include "thread.h"
#include <stdio.h>
#include "string.h"
#include "boat-monitor.h"

// static data variables

// static CannibalMonitor C("CannibalMonitor");
// static MissionaryMonitor M("MissionaryMonitor");
static BoatMonitor boat("BoatMonitor");

// Overloaded constructor
Cannibal::Cannibal(int c, int climit)
{
    // initialize
    cannibalNum = c;
    cannibalsLimit = climit;
}

Missionary::Missionary(int m, int mlimit)
{
    missionaryNum = m;
    missionariesLimit = mlimit;
    
}

Boat::Boat(int b)
{
    loadLimit = b;
}

// -------------------------------------------------------
// FUNCTION Cannibal::ThreadFunc()
// All executable code must be in function ThreadFunc()
// PARAMETER USAGE : None Used
// FUNCTION CALLED : goto_sleep(), food_ready(), Delay()
// --------------------------------------------------------
void Cannibal::ThreadFunc()
{
    Thread::ThreadFunc();
    char buffer[100];

    sprintf(buffer, "%*sCannibal %d starts\n", -cannibalNum, "", cannibalNum);
    write(1, buffer, strlen(buffer));

    //cannibalArray[cannibalNum] = 0; // Currently zero
    //cout << "Cannibal Array Element is " << cannibalArray[cannibalNum] << endl;
    
    while (1)
    {
        Delay();                                           // take a rest
        boat.CannibalArrives(cannibalNum, cannibalsLimit); // register to cross the river
        // other stuffs
        // come back for another river crossing
    }
}
// -------------------------------------------------------
// FUNCTION Missionary::ThreadFunc()
// All executable code must be in function ThreadFunc()
// PARAMETER USAGE : None Used
// FUNCTION CALLED : goto_sleep(), food_ready(), Delay()
// --------------------------------------------------------
void Missionary::ThreadFunc()
{
    Thread::ThreadFunc();
    char buffer[100];

    sprintf(buffer, "%*sMissionary %d starts\n", -missionaryNum, "", missionaryNum);
    write(1, buffer, strlen(buffer));

    //int missionaryArray[missionariesLimit + 1];
    //missionaryArray[missionaryNum] =  0; // Currently zero

    while (1)
    {
        Delay();                                                  // take a rest
        boat.MissionaryArrives(missionaryNum, missionariesLimit); // register to cross the river
        // other stuffs
        //missionaryArray[missionaryNum] = 0;
        // come back for another river crossing
    }
}
// -------------------------------------------------------
// FUNCTION Boat::ThreadFunc()
// All executable code must be in function ThreadFunc()
// PARAMETER USAGE : None Used
// FUNCTION CALLED : ready_to_eat(), finish_eating(), Delay()
// --------------------------------------------------------
void Boat::ThreadFunc()
{
    Thread::ThreadFunc();
    char buffer[100];
    int counter = 1;

    sprintf(buffer, "***** BOAT thread starts\n");
    write(1, buffer, strlen(buffer));

    while (1)
    {
        Delay();                                    // take a rest
        boat.BoatReady();                           // ready for the next round
        Delay();                                    // row the boat
        boat.BoatDone(loadLimit, "c1", "m1", "m3"); // all people are on the other side
        // come back for another river crossing
    }
}
