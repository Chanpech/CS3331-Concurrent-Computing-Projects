// ------------------------------------------------
// NAME : Chanpech Hoeng ID: choeng
// DUE DATE : 04/19/2023
// PROGRAM ASSIGNMENT #5
// FILE NAME : thread-main.cpp
// PROGRAM PURPOSE :
// Design a Hoare Style Monitor to implement a policy that has a single boat that
// cross the river, but it only seats three people, and must always carry a full load. In order to
// guarantee the safety of the missionaries, the boat cannot contains one missionary and two cannibals in
// the same boat (because the cannibals would gang up and eat the missionary), otherwise all other
// combinations are acceptable. 
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
    //cout << "cannibals = " << c_cannibals << endl;
    //cout << "missionaries = " << m_missionaries << endl;
    //cout << "boats = " << b_boats << endl;

    int *cannibalsArray = new int[c_cannibals];// allocate memory for the integers
    for (int i = 0; i < c_cannibals; i++)
    {
        cannibalsArray[i] = 0;
    }
    int *missionariesArray= new int[m_missionaries];// allocate memory for the integers
    for (int i = 0; i < m_missionaries; i++)
    {
        missionariesArray[i] = 0;
    }

    //Initialize Cannibal threads
    Cannibal *cannibal;
    for(int i = 1; i <= c_cannibals; i++){
        cannibal = new Cannibal(i, c_cannibals, cannibalsArray);         // i = cannibal number
        cannibal->Begin();                  //Begin the ThreadFunc()
    }

    //Initialize Missionary threads
    Missionary *missionary;
    for(int j = 1; j <= m_missionaries; j++){
        missionary = new Missionary(j, m_missionaries, missionariesArray);     // j = missionary number
        missionary->Begin();
    }

    //Initialize the single boat thread
    Boat *boat = new Boat(b_boats);     //Passing the limit load for parameter
    boat->Begin();


    //Joins the three threads
    for(int l = 1; l <=  c_cannibals; l++){
        cannibal->Join();
    }
    for(int l = 1; l <=  m_missionaries; l++){
        missionary->Join();
    }
    
    boat->Join();
    
    cout << "MONITOR: 10 crosses have been made." << endl;
    cout << "MONITOR: This river cross is closed indefinitely for renovation." << endl;

    Exit();
    return 0;
}