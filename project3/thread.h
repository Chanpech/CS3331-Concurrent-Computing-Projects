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
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

extern int **B; //Declared the 2 dimonional array B as global/extern variable

class thread : public Thread //thread is declared as a derived class of thread
{
    public:
        thread(int j, int i, int g); // column, row, gap
    private:
        //All executable code must be in function ThreadFunc()
        void ThreadFunc();
        int row;
        int column;
        int gap;
};