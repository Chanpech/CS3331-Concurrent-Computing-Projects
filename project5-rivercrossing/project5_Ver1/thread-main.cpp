// ------------------------------------------------
// NAME : Chanpech Hoeng ID: choeng
// DUE DATE : 03/17/2023
// PROGRAM ASSIGNMENT #5
// FILE NAME : thread-main.cpp
// PROGRAM PURPOSE :

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

int tempCounter;


int main(int argc, char *argv[])
{
    // You may assume that the command line always supply 3 integers, and these integers satisfy 0 < m <= n <= 20 and t > 0

    // Initialize variable
    int c_cannibals, m_missionaries, b_boats;

    c_cannibals = atoi(argv[1]);
    m_missionaries = atoi(argv[2]);
    b_boats = atoi(argv[3]);

    // Check to see if ({c, m, b} == 0) 
    if (c_cannibals == 0)
    {
        c_cannibals = 8;
    }
    if (m_missionaries == 0)
    {
        m_missionaries = 8;
    }
    if (b_boats == 0)
    {
        b_boats = 5;
    }
    
    // Prints will need to be deleted 
    cout << "cannibals = " << c_cannibals << endl;
    cout << "missionaries = " << m_missionaries << endl;
    cout << "boats = " << b_boats << endl;

    //Initialize Cannibal threads
    Cannibal *cannibal;
    for(int i = 1; i <= c_cannibals; i++){
        cannibal = new Cannibal(i);         // i = cannibal number
        cannibal->Begin();                  //Begin the ThreadFunc()
    }

    //Initialize Missionary threads
    Missionary *missionary;
    for(int j = 1; j <= m_missionaries; j++){
        missionary = new Missionary(j);     // j = missionary number
        missionary->Begin();
    }

    //Initialize Boat threads
    Boat *boat;
    for(int k = 1; k <= b_boats; k++){
        boat = new Boat(k);                 // k = boat number
        boat->Begin();
    }

    //Joins the three threads
    for(int l = 1; l <=  c_cannibals; l++){
        cannibal->Join();
    }
    for(int l = 1; l <=  m_missionaries; l++){
        missionary->Join();
    }
    for(int l = 1; l <= b_boats; l++){
        boat->Join();
    }    


    cout << "MONITOR: 10 crosses have been made." << endl;
    cout << "MONITOR: This river cross is closed indefinitely for renovation." << endl;

    Exit();
    return 0;
}