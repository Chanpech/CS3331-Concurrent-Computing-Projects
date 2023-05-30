// ------------------------------------------------
// NAME : Chanpech Hoeng ID: choeng
// DUE DATE : 04/19/2023
// PROGRAM ASSIGNMENT #5
// FILE NAME : thread-thread.cpp
// PROGRAM PURPOSE :
// Design a Hoare Style Monitor to implement a policy that has a single boat that
// cross the river, but it only seats three people, and must always carry a full load. In order to
// guarantee the safety of the missionaries, the boat cannot contains one missionary and two cannibals in
// the same boat (because the cannibals would gang up and eat the missionary), otherwise all other
// combinations are acceptable. 
// -------------------------------------------------

#include "ThreadClass.h"
#include <stdlib.h>
#include <iostream>
#include "thread.h"
#include <stdio.h>
#include "string.h"
#include "boat-monitor.h"


static BoatMonitor boat("BoatMonitor");

// Overloaded constructors
Cannibal::Cannibal(int c, int climit, int *carray)
{
    // initialize
    cannibalNum = c;
    cannibalsLimit = climit;
    cannibalsArray = carray;
}
Missionary::Missionary(int m, int mlimit, int *marray)
{
    missionaryNum = m;
    missionariesLimit = mlimit;
    missionaryArray = marray;
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
        boat.CannibalArrives(cannibalNum, cannibalsLimit, cannibalsArray); // register to cross the river
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
    //missionaryArray[mission cannibalsArray = new int[m_resource];// allocate memory for the integers
 

    while (1)
    {
        Delay();                                                  // take a rest
        boat.MissionaryArrives(missionaryNum, missionariesLimit, missionaryArray); // register to cross the river
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
        boat.BoatDone(loadLimit); // all people are on the other side
        // come back for another river crossing
    }
}
