// ------------------------------------------------
// NAME : Chanpech Hoeng ID: choeng
// DUE DATE : 04/6/2023
// PROGRAM ASSIGNMENT #4
// FILE NAME : thread-support.cpp
// PROGRAM PURPOSE : Contains all supporting functions such as
// CannibalArrives(), MissionaryArrives(), BoatReady(), BoatDone()
// -------------------------------------------------

#include "thread-support.h"
#include <stdlib.h>
#include <stdio.h>

void CannibalArrives(int cNum)
{
    char buff[250];
    sprintf(buff, "Cannibal %d Arrives\n", cNum);
    write(1, buff, strlen(buff));
}
void MissionaryArrives(int mNum)
{
    char buff[250];
    sprintf(buff, "Missionary %d arrives\n", mNum);
    write(1, buff, strlen(buff));
    
}
void BoatReady()
{
    char buff[250];
    sprintf(buff, "***** The boat is ready\n");
    write(1, buff, strlen(buff));
}
void BoatDone(int bNum)
{
    char buff[250];
    sprintf(buff, "***** Boat load (%d): Completed\n", bNum);
    write(1, buff, strlen(buff));
}