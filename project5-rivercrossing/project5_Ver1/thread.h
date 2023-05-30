// ------------------------------------------------
// NAME : Chanpech Hoeng ID: choeng
// DUE DATE : 03/17/2023
// PROGRAM ASSIGNMENT #5
// FILE NAME : thread.h 
// PROGRAM PURPOSE :
//
// -------------------------------------------------

#ifndef MY_FUNCTIONS_INCLUDED 
#define MY_FUNCTIONS_INCLUDED 

#include "ThreadClass.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#endif

extern int tempCounter;

class Cannibal : public Thread //thread is declared as a derived class of thread
{
    public:
        Cannibal(int c); 
    private:
        //All executable code must be in function ThreadFunc()
        void ThreadFunc();
        int cannibalNum;
};

class Missionary : public Thread //thread is declared as a derived class of thread
{
    public:
        Missionary(int m); 
    private:
        //All executable code must be in function ThreadFunc()
        void ThreadFunc();
        int missionaryNum;
};

class Boat : public Thread //thread is declared as a derived class of thread
{
    public:
        Boat(int b); 
    private:
        //All executable code must be in function ThreadFunc()
        void ThreadFunc();
        int boatNum;
};
