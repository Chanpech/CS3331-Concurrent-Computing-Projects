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


class Cannibal : public Thread //thread is declared as a derived class of thread
{
    public:
        Cannibal(int c, int climit); 
    private:
        //All executable code must be in function ThreadFunc()
        void ThreadFunc();
        int cannibalNum;
        int cannibalsLimit;
};

class Missionary : public Thread //thread is declared as a derived class of thread
{
    public:
        Missionary(int m, int mlimit); 
    private:
        //All executable code must be in function ThreadFunc()
        void ThreadFunc();
        int missionaryNum;
        int missionariesLimit;
};

class Boat : public Thread //thread is declared as a derived class of thread
{
    public:
        Boat(int b); 
    private:
        //All executable code must be in function ThreadFunc()
        void ThreadFunc();
        int loadLimit;
};
