// ------------------------------------------------
// NAME : Chanpech Hoeng ID: choeng
// DUE DATE : 04/6/2023
// PROGRAM ASSIGNMENT #4
// FILE NAME : thread.cpp
// PROGRAM PURPOSE :
// Initialize Mother eagle thread and Baby eagle threads. 
// Each ThreadFun() will create a random sequence with Delay() while
// utilizing the thread-support.cpp methods.
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
BabyEagle::BabyEagle(int num, int t)
{
    // initialize
    babyNum = num;
    limit = t;
}

// -------------------------------------------------------
// FUNCTION thread::ThreadFunc()
// All executable code must be in function ThreadFunc()
// PARAMETER USAGE : None Used
// FUNCTION CALLED : goto_sleep(), food_ready(), Delay()
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
        food_ready(counter);        // make food available
        Delay();                    // do something else
    }

}
// -------------------------------------------------------
// FUNCTION thread::ThreadFunc()
// All executable code must be in function ThreadFunc()
// PARAMETER USAGE : None Used
// FUNCTION CALLED : ready_to_eat(), finish_eating(), Delay()
// --------------------------------------------------------
void BabyEagle::ThreadFunc()
{
    Thread::ThreadFunc();
    char buffer[100];
    sprintf(buffer, "%*sBaby eagle %d started.\n", -babyNum, "", babyNum);
    write(1, buffer, strlen(buffer));
    while(counter <= limit) {
        Delay();                    //play for a while
        ready_to_eat(babyNum, limit);      //get hungry
        Delay();                    //eat for while
        Delay();
                                    // you may call Delay() multiple times 
        finish_eating(babyNum);     // finsih eating 
                                    //do some other thing

    }
}
