// ------------------------------------------------
// NAME : Chanpech Hoeng ID: choeng
// DUE DATE : 03/17/2023
// PROGRAM ASSIGNMENT #3
// FILE NAME : thread.h 
// PROGRAM PURPOSE :
// The prefix sum program takes in input files of size n and an array of integers.
// To produce the sequence the program utilizes ThreadMentor threads to handle it computation.
// Although the program ran at O(n log(n)), it uses threads to computate concurrently. 
// -------------------------------------------------

#ifndef MY_FUNCTIONS_INCLUDED 
#define MY_FUNCTIONS_INCLUDED 

#include "ThreadClass.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


#endif

extern int *feedingPots; //Declared the 1 dimonional array feedingPots as global/extern variable
extern int counter;
extern int num_child;
extern int limit;
extern int totalPots;
extern bool motherAwake;


class Mother : public Thread //thread is declared as a derived class of thread
{
    public:
        Mother(int cycleCounter, int feedLimit, bool sleep); // column, row, gap
    private:
        //All executable code must be in function ThreadFunc()
        void ThreadFunc();
        int cycleCounter;
        int feedLimit;
        bool sleep;
};

class BabyEagle : public Thread //thread is declared as a derived class of thread
{
    public:
        BabyEagle(int babyNum); 
    private:
        //All executable code must be in function ThreadFunc()
        void ThreadFunc();
        int babyNum;
};