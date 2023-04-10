// ------------------------------------------------
// NAME : Chanpech Hoeng ID: choeng
// DUE DATE : 03/17/2023
// PROGRAM ASSIGNMENT #3
// FILE NAME : thread-main.cpp (your unix file name)
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

// Overload Constructor 
thread::thread(int j, int i, int g)
{
    row = i;
    column = j;
    gap = g;
}

// -------------------------------------------------------
// FUNCTION thread::ThreadFunc()
// All executable code must be in function ThreadFunc()
// PARAMETER USAGE : None Used
// FUNCTION CALLED : None Used
// --------------------------------------------------------
void thread::ThreadFunc()
{
    Thread::ThreadFunc();
    /*
    //write(1, "thread\n", 8);
    if(B[row - 1][column - gap] == 0){
        B[row - 1][column - gap] = B[row - 2][column - gap];
    }
    B[row][column] +=  B[row - 1][column] + B[row - 1][column - gap];
    */
   
    char buffer[100];
    sprintf(buffer, "Thread %d Created\n", column);
    write(1, buffer, strlen(buffer));
    if (column - gap < 0)
    {
        B[row][column] = B[row - 1][column];
        sprintf(buffer, "Thread %d copies %d\n", column, B[row-1][column]);
        write(1, buffer, strlen(buffer));
    }
    else
    {
        B[row][column] = B[row - 1][column] + B[row - 1][column - gap];
        sprintf(buffer, "Thread %d computes %d + %d\n",column, B[row - 1][column],  B[row - 1][column - gap]);
        write(1, buffer, strlen(buffer));
    }
    sprintf(buffer, "Thread %d exits\n", column);
    write(1, buffer, strlen(buffer)); 
}