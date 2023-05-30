// ------------------------------------------------
// NAME : Chanpech Hoeng ID: choeng
// DUE DATE : 04/19/2023
// PROGRAM ASSIGNMENT #5
// FILE NAME : boat-monitor.cpp
// PROGRAM PURPOSE :
// Design a Hoare Style Monitor to implement a policy that has a single boat that
// cross the river, but it only seats three people, and must always carry a full load. In order to
// guarantee the safety of the missionaries, the boat cannot contains one missionary and two cannibals in
// the same boat (because the cannibals would gang up and eat the missionary), otherwise all other
// combinations are acceptable. 
// -------------------------------------------------
#include <iostream>
#include "boat-monitor.h"
#include "ThreadClass.h"
#include <queue>

//Initalize the variables in the BoatMonitor constructor
BoatMonitor::BoatMonitor(char *Name)
    : Monitor(Name, HOARE), BoatQueue("BoatQueue"), CannibalQueue("CannibalQueue"), MissionaryQueue("MissionaryQueue"), LoadQueue("LoadQueue")
{
    counter = 1;
    isEmpty = true;

    arrivedCannibals = 0;
    arrivedMissionaries = 0;

    cannibalsLimit =0;
    missionariesLimit = 0;

    cannibalReturned =0;
    missionaryReturned =0;

    threeCannibalsSelected = false;
    threeMissionariesSelected = false;
    mixedSelection = false;
}
int BoatMonitor::findAvailableCannibal(){
    if(!cannibalQueue.empty()){
        int number = cannibalQueue.front();
        cannibalQueue.pop();
        return number;
    }
    return -1;
}
int BoatMonitor::findAvailableMissionary(){
    if(!missionaryQueue.empty()){
        int number = missionaryQueue.front();
        missionaryQueue.pop();
        return number;
    }
    return -1;
}
// -------------------------------------------------------
// FUNCTION BoatMonitor :: ThreeCannibals(...)
// Purpose: 
// PARAMETER USAGE : 
//      int c1, int c2, int c3
// FUNCTION CALLED :
//      CannibalQueue.Signal();
// -------------------------------------------------------
void BoatMonitor::ThreeCannibals()
{
    int c1, c2, c3;

    MonitorBegin();
        c1 = findAvailableCannibal();
        c2 = findAvailableCannibal();
        c3 = findAvailableCannibal();

        passengerQueue.push(c1);
        passengerQueue.push(c2);
        passengerQueue.push(c3);

        char buff[250];
        sprintf(buff, "MONITOR(%d): three cannibals (%d, %d, %d) are selected\n", counter, c1, c2, c3);
        write(1, buff, strlen(buff));

        threeCannibalsSelected = true;
        //cannibalReturned = arrivedCannibals;
        arrivedCannibals = arrivedCannibals - 3; // new arrived cannibals
    MonitorEnd();

    for(int i = 0; i < 3; i++){
        CannibalQueue.Signal();
    }
}
// -------------------------------------------------------
// FUNCTION BoatMonitor :: ThreeMssionaries
// Purpose: 
// PARAMETER USAGE :
//      int m1, int m2, int m
// FUNCTION CALLED :
//      MissionaryQueue.Signal()
// -------------------------------------------------------
void BoatMonitor::ThreeMissionaries()
{   
    int m1,m2,m3;
    MonitorBegin();
        char buff[250];
        m1 = findAvailableMissionary();
        m2 = findAvailableMissionary();
        m3 = findAvailableMissionary();
        
        passengerQueue.push(m1);
        passengerQueue.push(m2);
        passengerQueue.push(m3);

        sprintf(buff, "MONITOR(%d): three missionaries (%d, %d, %d) are selected\n", counter, m1, m2, m3);
        write(1, buff, strlen(buff));

        threeMissionariesSelected = true;
        //missionaryReturned = arrivedMissionaries;
        arrivedMissionaries = arrivedMissionaries - 3; // new arrived missionaries;
    MonitorEnd();

    for(int i = 0; i < 3; i++)
        MissionaryQueue.Signal();
    
}
// -------------------------------------------------------
// FUNCTION BoatMonitor :: OneCannibalsTwoMissionaries
// Purpose: 
// PARAMETER USAGE :
//      int cNum, int m1, int m2
// FUNCTION CALLED :
//      CannibalQueue.Signal(), MissionaryQueue.Signal()
// -------------------------------------------------------
void BoatMonitor::OneCannibalsTwoMissonaries()
{
    int c1, m1, m2;
    MonitorBegin();
        char buff[250];

        c1 = findAvailableCannibal();
        m1 = findAvailableMissionary();
        m2 = findAvailableMissionary();

        passengerQueue.push(c1);
        passengerQueue.push(m1);
        passengerQueue.push(m2);

        sprintf(buff, "MONITOR(%d): one cannibal (%d) and two missionaries (%d, %d) are selected\n", counter, c1, m1, m2);
        write(1, buff, strlen(buff));

        mixedSelection = true;
        //cannibalReturned = arrivedCannibals;
        //missionaryReturned = arrivedMissionaries;
        arrivedCannibals--;
        arrivedMissionaries = arrivedMissionaries - 2;
    MonitorEnd(); 
    
    CannibalQueue.Signal();
    MissionaryQueue.Signal();
    MissionaryQueue.Signal();
}
// -------------------------------------------------------
// FUNCTION BoatMonitor::CrossesMade
// Purpose: 
// PARAMETER USAGE :
//      int num
// FUNCTION CALLED :
//      Exit();
// -------------------------------------------------------
void BoatMonitor::CrossesMade(int num)
{
    MonitorBegin();
        char buff[250];
        sprintf(buff, "MONITOR: %d crosses have been made\n", num);
        write(1, buff, strlen(buff));

        // Might move
        sprintf(buff, "MONITOR: This river cross is closed indefinitely for renovation.\n");
        write(1, buff, strlen(buff));
        
        Exit();
    MonitorEnd();
}

// -------------------------------------------------------
// FUNCTION BoatMonitor::SelectThreePassenger
// Purpose: 
// PARAMETER USAGE :
//      None
// FUNCTION CALLED :
//      ThreeCannibals, OneCannibalsTwoMissioanries, ThreeMissionaries
// -------------------------------------------------------
void BoatMonitor::SelectThreePassenger()
{
    if (arrivedCannibals >= 3 && (arrivedCannibals % 2) == 0 )
    {
        ThreeCannibals();
    }
    else if (arrivedMissionaries >= 2 && arrivedCannibals >= 1 && (arrivedMissionaries % 2) == 0)
    {
        OneCannibalsTwoMissonaries();
    }
    else if (arrivedMissionaries >= 3)
    {
        ThreeMissionaries();
    }else{
        SelectThreePassenger();
    } 

    MonitorBegin();
        isEmpty = false;
    /* //Used to ensure that three threads have have arrived before beginning another load. But it deadlock.
    if(arrivedCannibals != cannibalReturned || arrivedMissionaries != missionaryReturned){
        LoadQueue.Wait();
    }*/
      MonitorEnd();

}


void BoatMonitor::BoatLoadComplete(){
    MonitorBegin();
        char buff[250];
        sprintf(buff, "***** Boat load (%d): Completed\n", counter);
        write(1, buff, strlen(buff));
        BoatQueue.Signal();
    MonitorEnd();
}
// -------------------------------------------------------
// FUNCTION BoatMonitor::BoatReady()
// Purpose: 
// PARAMETER USAGE :
// 
// FUNCTION CALLED :
// 
// -------------------------------------------------------
void BoatMonitor::BoatReady()
{
    MonitorBegin();
    // Boat is ready when the counter is 0
    if (!isEmpty)
    {
        MonitorEnd();
        BoatQueue.Wait(); // Block the boat from going if it is not empty yet
    }
    char buff[250];
    sprintf(buff, "***** The boat is ready\n");
    write(1, buff, strlen(buff));
    MonitorEnd();

    SelectThreePassenger();
}
// -------------------------------------------------------
// FUNCTION BoatMonitor::BoatDone
// Purpose: 
// PARAMETER USAGE :
//      int boatLimit, string p1, string p2, string p3
// FUNCTION CALLED :
//      MonitorEnd(), CrossesMade(counter)
// -------------------------------------------------------
void BoatMonitor::BoatDone(int boatLimit)
{
    MonitorBegin();
        isEmpty = true;
        char buff[250];

        int p1 =  passengerQueue.front();
        passengerQueue.pop();
        int p2 =  passengerQueue.front();
        passengerQueue.pop();
        int p3 = passengerQueue.front();
        passengerQueue.pop();

    if(mixedSelection){
        sprintf(buff, "***** Boat load (%d): Passenger list (c%d, m%d, m%d)\n", counter, p1, p2, p3);
        write(1, buff, strlen(buff));
    } else if(threeCannibalsSelected){
        sprintf(buff, "***** Boat load (%d): Passenger list (c%d, c%d, c%d)\n", counter, p1, p2, p3);
        write(1, buff, strlen(buff));
    }else if(threeMissionariesSelected){
        sprintf(buff, "***** Boat load (%d): Passenger list (m%d, m%d, m%d)\n", counter, p1, p2, p3);
        write(1, buff, strlen(buff));
    }
    
    // Free the randomized set

    if (counter == boatLimit)
    {
        MonitorEnd();
        BoatLoadComplete();
        CrossesMade(counter);
    }
    counter++;
    MonitorEnd();

    BoatLoadComplete();
}

void BoatMonitor::SetCannibalsLimit(int climit){
    cannibalsLimit = climit;
}
void BoatMonitor::SetMissionariesLimit(int mlimit){
    missionariesLimit = mlimit;
}
// -------------------------------------------------------
// FUNCTION BoatMonitor::CannibalArrives
// Purpose: 
// PARAMETER USAGE :    
//      int cNum, int climit
// FUNCTION CALLED :
//      CannibalQueue.Wait();
// -------------------------------------------------------
void BoatMonitor::CannibalArrives(int cNum, int climit, int *cArray)
{
    MonitorBegin();
        char buff[250];
        sprintf(buff, "%*sCannibal %d Arrives\n", -cNum, "", cNum);
        write(1, buff, strlen(buff));
        SetCannibalsLimit(climit);
        //cannibalArray[cNum] = 1;
        //cannibals[cNum] = cannibalArray[cNum];
        cannibalQueue.push(cNum);
        arrivedCannibals++;
        /*
        if(arrivedCannibals >= cannibalReturned && arrivedMissionaries >= missionaryReturned){
            LoadQueue.Signal();
        }*/
        CannibalQueue.Wait();
    MonitorEnd();
}

// -------------------------------------------------------
// FUNCTION BoatMonitor::MissionaryArrives
// Purpose: 
// PARAMETER USAGE :
//      int mNum, int mlimit
// FUNCTION CALLED :
//        MissionaryQueue.Wait();
// -------------------------------------------------------
void BoatMonitor::MissionaryArrives(int mNum, int mlimit, int *mArray)
{
    MonitorBegin();
    char buff[250];
    sprintf(buff, "%*sMissionary %d arrives\n", -mNum, "", mNum);
    write(1, buff, strlen(buff));
    SetMissionariesLimit(mlimit);
    missionaryQueue.push(mNum);

    arrivedMissionaries++;
    /*
    if(arrivedCannibals >= cannibalReturned && arrivedMissionaries >= missionaryReturned){
            LoadQueue.Signal();
    } */

    MissionaryQueue.Wait();

    MonitorEnd();
}