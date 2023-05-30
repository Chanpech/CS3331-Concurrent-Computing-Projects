#include <iostream>
#include "boat-monitor.h"
#include "ThreadClass.h"

BoatMonitor::BoatMonitor(char *Name)
    : Monitor(Name, HOARE), BoatQueue("BoatQueue"), CannibalQueue("CannibalQueue"), MissionaryQueue("MissionaryQueue"), LoadQueue("LoadQueue")
{
    Limit = 3;
    counter = 1;
    isEmpty = true;

    arrivedCannibals = 0;
    arrivedMissionaries = 0;

/*
    threePassenger[0] = "";
    threePassenger[1] = "";
    threePassenger[2] = "";
/*
    for (int i = 0; i < 3; i++)
    {
        threePassenger[i] = (char*) malloc(sizeof(char) * (50)); //The length of each buffer is 50 long
    } */
}

void BoatMonitor::ThreeCannibals(int c1, int c2, int c3)
{
    MonitorBegin();
        char buff[250];
        sprintf(buff, "MONITOR(%d): three cannibals (%d, %d, %d) are selected\n", counter, c1, c2, c3);
        write(1, buff, strlen(buff));

        /*
        char c1Buf[50];
        char c2Buf[50];
        char c3Buf[50];

        sprintf(c1Buf, "c%d", c1);
        sprintf(c2Buf, "c%d", c2);
        sprintf(c3Buf, "c%d", c3);
        */
       /*
        threePassenger[0] = "c1";
        threePassenger[1] = "c1";
        threePassenger[2] = "c3";
        */

        arrivedCannibals = arrivedCannibals - 3; // new arrived cannibals
    /*
    for(int i = 0; i < 3; i++){
        char buffer[250];
        sprintf(buffer, "%s", threePassenger[i]);
        write(1, buffer, strlen(buffer));
    }*/
    MonitorEnd();

    for(int i = 0; i < 3; i++){
        CannibalQueue.Signal();
    }
}

void BoatMonitor::ThreeMissionaries(int m1, int m2, int m3)
{
    MonitorBegin();
        char buff[250];
        sprintf(buff, "MONITOR(%d): three missionaries (%d, %d, %d) are selected\n", counter, m1, m2, m3);
        write(1, buff, strlen(buff));
        
        arrivedMissionaries = arrivedMissionaries - 3; // new arrived missionaries;
    MonitorEnd();

    for(int i = 0; i < 3; i++)
        MissionaryQueue.Signal();
    
}

void BoatMonitor::OneCannibalsTwoMissonaries(int cNum, int m1, int m2)
{
    MonitorBegin();
        char buff[250];
        sprintf(buff, "MONITOR(%d): one cannibal (%d) and two missionaries (%d, %d) are selected\n", counter, cNum, m1, m2);
        write(1, buff, strlen(buff));

        arrivedCannibals--;
        arrivedMissionaries = arrivedMissionaries - 2;
    MonitorEnd();
    
    CannibalQueue.Signal();
    MissionaryQueue.Signal();
    MissionaryQueue.Signal();
}

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

int BoatMonitor::Increment()
{
    int number;

    MonitorBegin();
    counter++;
    number = counter;
    MonitorEnd();
    return number;
}

// Should need this
int BoatMonitor::Decrement()
{
    int number;

    MonitorBegin();
    counter--;
    number = counter;
    MonitorEnd();
    return number;
}

void BoatMonitor::SelectThreePassenger()
{
    if (arrivedCannibals >= 3)
    {
        ThreeCannibals(4, 3, 2);
    }
    else if (arrivedMissionaries >= 2 && arrivedCannibals >= 1)
    {
        OneCannibalsTwoMissonaries(4, 3, 2);
    }
    else if (arrivedMissionaries >= 3)
    {
        ThreeMissionaries(4, 3, 2);
    }  
    /*
    MonitorBegin();

            sprintf(buff, "Three Passenger:\n");
            write(1, buff, strlen(buff));
        for(int i = 0; i < 3; i++){
            sprintf(buff, "Passenger[%d] is %s\n", i, cannibals[i]);
            write(1, buff, strlen(buff));
        }
    MonitorEnd();*/

}
void BoatMonitor::BoatReady()
{
    MonitorBegin();
    // Boat is ready when the counter is 0
    if (!isEmpty)
    {
        BoatQueue.Wait(); // Block the boat from going if it is not empty yet
    }
    char buff[250];
    sprintf(buff, "***** The boat is ready\n");
    write(1, buff, strlen(buff));
    LoadQueue.Signal();
    MonitorEnd();

    SelectThreePassenger();
}

void BoatMonitor::BoatDone(int boatLimit, string p1, string p2, string p3)
{
    MonitorBegin();
    isEmpty = true;

    char buff[250];
    sprintf(buff, "***** Boat load (%d): Passenger list (%s, %s, %s)\n", counter, "c1", "m1", "m2");
    write(1, buff, strlen(buff));

    sprintf(buff, "***** Boat load (%d): Completed\n", counter);
    write(1, buff, strlen(buff));
    
    // Free the randomized set

    if (counter == boatLimit)
    {
        MonitorEnd();
        CrossesMade(counter);
    }
    counter++;
    MonitorEnd();
}

/*
void BoatMonitor::BoatLoad(int bNums, string p1, string p2, string p3)
{
    MonitorBegin();
        if(!isEmpty){
            LoadQueue.Wait();
        }
        char buff[250];
        sprintf(buff, "***** Boat load (%d): Passenger list (%s, %s, %s)n", bNum, "c1", "m1", "m2");
        write(1, buff, strlen(buff));
    MonitorEnd();
}*/

// Cannibal method
void BoatMonitor::CannibalArrives(int cNum, int climit, int *missionariesArray)
{
    MonitorBegin();
    char buff[250];
    sprintf(buff, "%*sCannibal %d Arrives\n", -cNum, "", cNum);
    write(1, buff, strlen(buff));

    //cannibalArray[cNum] = 1;
    //cannibals[cNum] = cannibalArray[cNum];
    
    arrivedCannibals++;
    CannibalQueue.Wait();
    MonitorEnd();
}

// Missionary method
void BoatMonitor::MissionaryArrives(int mNum, int mlimit, int *cannibalArray)
{
    MonitorBegin();
    char buff[250];
    sprintf(buff, "%*sMissionary %d arrives\n", -mNum, "", mNum);
    write(1, buff, strlen(buff));
    arrivedMissionaries++;
    MissionaryQueue.Wait();
    /*
    if (numberOfCannibal == 5)  // buffer is full

    numberOfCannibal++;*/
    MonitorEnd();
}