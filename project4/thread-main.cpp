// ------------------------------------------------
// NAME : Chanpech Hoeng ID: choeng
// DUE DATE : 03/17/2023
// PROGRAM ASSIGNMENT #3
// FILE NAME : thread-main.cpp
// PROGRAM PURPOSE :
// The program design a game which describes a family of eagles consisting of one mother eagle, 
// n baby eagles, and m feeding pots. The mother eagle does not require a feeding pot and 
// all feeding pots are empty at the start. Each baby eagle must eat using a feeding pot, 
// and no more than m baby eagles can eat at the same time since each pot can only serve one baby eagle. 
// When a baby eagle wants to eat, it must wait until a feeding pot has food. After eating, 
// the corresponding feeding pot becomes empty and must be refilled by the mother eagle. 
// The mother eagle sleeps until a baby eagle wakes her up, and she then hunts for food and fills all feeding pots at once. 
// If a baby eagle finds all feeding pots empty, it wakes the mother eagle up, and she refills the pots before sleeping again. 
// Only one baby eagle is allowed to wake the mother eagle up, even if multiple hungry baby eagles find all pots empty.
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
int totalPots;
bool motherAwake;

int main(int argc, char *argv[])
{
    // You may assume that the command line always supply 3 integers, and these integers satisfy 0 < m <= n <= 20 and t > 0
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
    
    totalPots = m_resource;

    bool motherAwake = false;
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
        babyeagle = new BabyEagle(i, t_occurrence);
        babyeagle->Begin();
    }
    // Join the BabyEagle threads
    for (int j = 0; j < n_childs; j++)
    {
        babyeagle->Join();
    }

    mother->Join();
    cout << "Mother eagle retires after serving " << t_occurrence << " feedings. Game ends!!!" << endl;

    Exit(); 
    return 0;
}