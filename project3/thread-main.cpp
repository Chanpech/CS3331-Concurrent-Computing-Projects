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
#include <iostream>
#include "ThreadClass.h"
#include "thread.h"
#include "string.h"
#include <math.h>
#include <sstream> 
#include <stdlib.h>
using namespace std;

int **B; // Global variable which was declared as extern in the header file thread.h

int main(int argc, char *argv[])
{
    // Initialize variables
    int j, i;
    int size; // also known as n
    int k;
    int stage, gap;
    char buffer[50];

    // Reads in input file into the declared variables.
    cin >> size;
    int initialArray[size];
    k = log2(size);
    B = new int *[k + 1];

    cout << "Number of input data = " << size << endl;
    cout << "Input array: " << endl;

    // Store the integers into initialArray.
    for (int i = 0; i < size; i++)
    {
        cin >> initialArray[i];
        cout << initialArray[i] << " ";
    }
    cout << endl;

    // dynamically allocate memory to B
    for (int i = 0; i <= k; ++i)
    {
        B[i] = new int[size];
    }
    // initialize the first row of B[*,*]
    for (int i = 0; i < size; i++)
    {
        B[0][i] = initialArray[i];
    }

    // prefix calculation
    // Branching off the main thread we create the threads[]
    thread **threads = new thread *[size];

    // To make the threads run concurrently the Begin() and Join() have to be declared separately.
    // If join were to be declared in the same nested loop as below, then the program will be serialized.
    // This is because main thread immediately continue to want to join threads[i].
    cout << "Run i" << endl;
   for (i = 1, gap = 1; i <= k; i++, gap *= 2) // Rows
    {
        for (j = 0; j < size; j++) // Columns
        {
            threads[j] = new thread(j, i, gap);
            threads[j]->Begin();
        }
        for (j = 0; j < size; j++) // Columns
        {
            threads[j]->Join();
        }
        sprintf(buffer, "Result after run %d:\n", i);
        write(1, buffer, strlen(buffer));
        memset(buffer, 0, sizeof(buffer));
        char temp[50];
        for (j = 0; j < size; j++){
            sprintf(temp, "%d ", B[i][j]);
            strcat(buffer,temp);
        }
        strcat(buffer,"\n");
        write(1, buffer, strlen(buffer));
    } 

    cout << "Final result after run k -> " << k << endl;
    for (i = 0; i < size; i++)
    {
        cout << B[k][i] << " ";
    }
    cout << endl;
    Exit();
    return 0;
}