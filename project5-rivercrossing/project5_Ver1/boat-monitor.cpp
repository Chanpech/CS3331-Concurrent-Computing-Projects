#include <iostream>
#include "boat-monitor.h"
#include "ThreadClass.h"

BoatMonitor::BoatMonitor(char *Name)
            : Monitor(Name, HOARE), BoatQueue("BoatQueue"), CannibalQueue("CannibalQueue"), MissionaryQueue("MissionaryQueue")
{
    Limit = 3;
    counter = 0;
    isEmpty = true;
}

void BoatMonitor::ThreeCannibals(int monNum, int c1, int c2, int c3){
    char buff[250];
    sprintf(buff, "MONITOR(%d): three cannibals (%d, %d, %d) are selected", monNum, c1, c2, c3);
    write(1, buff, strlen(buff));
}

void BoatMonitor::ThreeMissionaries(int monNum, int m1, int m2, int m3){

    char buff[250];
    sprintf(buff, "MONITOR(%d): three missionaries (%d, %d, %d) are selected", monNum, m1, m2, m3);
    write(1, buff, strlen(buff));
}

void BoatMonitor::OneCannibalsTwoMissonaries(int monNum, int cNum, int m1, int m2){

    char buff[250];
    sprintf(buff, "MONITOR(%d): one cannibal (%d) and two missionaries (%d, %d) are selected", monNum, cNum, m1, m2);
    write(1, buff, strlen(buff));
}

void BoatMonitor::CrossesMade(int num){
    
    char buff[250];
    sprintf(buff, "MONITOR: %d crosses have been made", num);
    write(1, buff, strlen(buff));

    //Might move
    sprintf(buff, "MONITOR: This river cross is closed indefinitely for renovation.");
    write(1, buff, strlen(buff));
}

int BoatMonitor::Increment(){
    int number;
    
    MonitorBegin();
        counter++;
        number = counter;
    MonitorEnd();
    return number;
}

//Should need this 
int BoatMonitor::Decrement(){
    int number;

    MonitorBegin();
        counter--;
        number = counter;
    MonitorEnd();
    return number;
}


void  BoatMonitor::BoatReady()
{
    MonitorBegin();
        //Boat is ready when the counter is 0
        if(!isEmpty){  
            BoatQueue.Wait(); //Block the boat from going if it is not empty yet 
        }         
        char buff[250];
        sprintf(buff, "***** The boat is ready\n");
        write(1, buff, strlen(buff));
            
    MonitorEnd();
}

void  BoatMonitor::BoatDone(int bNum)
{
    MonitorBegin();
        isEmpty = true;
        char buff[250];
        sprintf(buff, "***** Boat load (%d): Completed\n", bNum);
        write(1, buff, strlen(buff));
        counter++;
        if(counter == Limit){
            Exit();//End the process
        }
    MonitorEnd();
}


// Cannibal method
void BoatMonitor::CannibalArrives(int cNum)
{
    MonitorBegin();
        char buff[250];
        sprintf(buff, "%*sCannibal %d Arrives\n", -cNum, "", cNum);
        write(1, buff, strlen(buff));
        CannibalQueue.Wait();
    MonitorEnd();
}

// Missionary method
void BoatMonitor::MissionaryArrives(int mNum)
{
    MonitorBegin();
        char buff[250];
        sprintf(buff, "%*sMissionary %d arrives\n", -mNum, "", mNum);
        write(1, buff, strlen(buff));
        MissionaryQueue.Wait();
        /*
        if (numberOfCannibal == 5)  // buffer is full
           
        numberOfCannibal++;*/
    MonitorEnd();
}