// ------------------------------------------------
// NAME : Chanpech Hoeng ID: choeng
// DUE DATE : 03/17/2023
// PROGRAM ASSIGNMENT #3
// FILE NAME : thread.cpp
// PROGRAM PURPOSE :
// The prefix sum program takes in input files of size n and an array of integers.
// To produce the sequence the program utilizes ThreadMentor threads to handle it computation.
// Although the program ran at O(n log(n)), it uses threads to computate concurrently.
// -------------------------------------------------

#include "ThreadClass.h"
#include <stdlib.h>
#include <iostream>
#include "thread.h"
#include <stdio.h>
#include "string.h"
#include "thread-support.cpp"

/*
Ex.
static Semaphore FourChairs("FourChairs", NUM_OF_PHILOSOPHERS - 1);
Mutex Block("Lock");

Semaphore *Block;
Block = new Semaphore("MyLock", 1);
*/

// Overloaded constructor (int cycleCounter, int feedLimit, int sleep)
Mother::Mother(int cycle, int t, bool asleep)
{
    cycleCounter = cycle;
    feedLimit = t;
    sleep = asleep;
}

// Overloaded constructor
BabyEagle::BabyEagle(int num)
{
    // initialize
    babyNum = num;
}

// -------------------------------------------------------
// FUNCTION thread::ThreadFunc()
// All executable code must be in function ThreadFunc()
// PARAMETER USAGE : None Used
// FUNCTION CALLED : None Used
// --------------------------------------------------------
void Mother::ThreadFunc()
{
    Thread::ThreadFunc();
    char buffer[100];
    sprintf(buffer, "Mother eagle started.\n");
    write(1, buffer, strlen(buffer));
    while(counter < feedLimit){
        goto_sleep();               // take a nap
        Delay();                    // prepare food
        //You may call delay() multiple times (use a random number generator) to have alonger and more random delay.

        // wait for a baby eagle to wake mother up
        food_ready(counter);        // make food available
        Delay();                    // do something else
    }

}
// -------------------------------------------------------
// FUNCTION thread::ThreadFunc()
// All executable code must be in function ThreadFunc()
// PARAMETER USAGE : None Used
// FUNCTION CALLED : None Used
// --------------------------------------------------------
void BabyEagle::ThreadFunc()
{
    Thread::ThreadFunc();
    char buffer[100];
    sprintf(buffer, "%*sBaby eagle %d started.\n", -babyNum, "", babyNum);
    write(1, buffer, strlen(buffer));
    while(counter < limit) {
        Delay();            //play for a while
        ready_to_eat(babyNum);     //get hungry
        Delay();            //eat for while
        Delay();
        // you may call Delay() multiple times 
        finish_eating(babyNum);    // finsih eating 
        //do some other thing

    }
}
