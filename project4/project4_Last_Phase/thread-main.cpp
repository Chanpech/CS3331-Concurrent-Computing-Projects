// ------------------------------------------------
// NAME : Chanpech Hoeng ID: choeng
// DUE DATE : 03/17/2023
// PROGRAM ASSIGNMENT #3
// FILE NAME : thread-main.cpp
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
#include <array>

using namespace std;

int *feedingPots;
int counter;
int num_child;
int limit;
int totalPots;
bool motherAwake;

int main(int argc, char *argv[])
{
    // You may assume that the command line always supply 3 integers,
    // and these integers satisfy 0 < m <= n <= 20 and t > 0
    // Initialize variable
    int m_resource, n_childs, t_occurrence;
    m_resource = atoi(argv[1]);
    n_childs = atoi(argv[2]);
    t_occurrence = atoi(argv[3]);
    counter = 0;
    num_child = n_childs;
    // Check to see if ({m,n,t} == 0) initialize to zero
    if (m_resource == 0)
    {
        m_resource = 10;
    }
    if (n_childs == 0)
    {
        n_childs = 10;
    }
    if (t_occurrence == 0)
    {
        t_occurrence = 10;
    }
    limit = t_occurrence;
    totalPots = m_resource;

    cout << "m = " << m_resource << endl;
    cout << "n = " << n_childs << endl;
    cout << "t = " << t_occurrence << endl;

    cout << "MAIN: There are " << n_childs << " baby eagles, "
         << m_resource << " feeding pots, and "
         << t_occurrence << " feedings" << endl;

    cout << "MAIN: Game starts!!!!!" << endl;

    // Mother retires but let all baby eagles finish eating

    // Initialize feedingPots and set everything to 0 -> empty
    feedingPots = new int[m_resource];// allocate memory for the integers
    for (int i = 0; i < m_resource; i++)
    {
        feedingPots[i] = 0;
    }

    // Initialize Mother thread
    Mother *mother = new Mother(1, t_occurrence, false); //(int cycleCounter, int feedLimit, int sleep)
    mother->Begin();

    // Initialize BabyEagle threads
    BabyEagle *babyeagle;
    for (int i = 1; i <= n_childs; i++)
    {
        babyeagle = new BabyEagle(i);
        babyeagle->Begin();
    }
    for (int j = 0; j < n_childs; j++)
    {
        babyeagle->Join();
    }

    cout << "counter = " << counter << endl;
    mother->Join();
    cout << "Mother eagle retires after serving " << t_occurrence << " feedings. Game ends!!!" << endl;

    // Threads should still
    Exit(); 
    return 0;
}