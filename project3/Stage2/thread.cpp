//-------------------------------------------------------
// NAME : John Smith User ID: xxxxxxxx
// DUE DATE : mm/dd/yyyy
// PROGRAM ASSIGNMENT #
// FILE NAME : xxxx.yyyy.zzzz (your unix file name)
// PROGRAM PURPOSE :
// A couple of lines describing your program briefly
// ----------------------------------------------------

#include "ThreadClass.h"
#include <stdlib.h>
#include <iostream>
#include "thread.h"
#include <stdio.h>

//class::method
//This allow you to distinguish variables with the same name

thread::thread(int Size, int K, int PrefixSumArray[][])
        :size(Size), k(K), prefixSumArray(PrefixSumArray)
{
    executions = Size;//something
}

void thread::ThreadFunc()
{
    Thread::ThreadFunc();
    for(int i = 0; i < executions; i++){
        write(1, "thread\n", 8);
    }
    int size, stage, gap, i, k;


    for (stage = 1, gap = 1; stage <= k; stage++, gap *= 2)
    {
        for (i = 0; i < size; i++)
        {
            //Separate out the thread task here
            
            if (i - gap < 0)
                prefixSumArray[stage][i] = prefixSumArray[stage-1][i];
            else
                prefixSumArray[stage][i] = prefixSumArray[stage - 1][i] + prefixSumArray[stage - 1][i-gap];
            cout << prefixSumArray[stage][i] << " ";
        }
        cout << endl;
    }


    Exit(); //Terminates thread
}