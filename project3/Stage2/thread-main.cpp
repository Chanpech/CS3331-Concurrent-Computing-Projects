// -------------------------------------------------------
// FUNCTION xxyyzz : (function name)
// the purpose of this function
// PARAMETER USAGE :
// a list of all parameters and their meaning
// FUNCTION CALLED :
#include <iostream>
#include "ThreadClass.h"
#include "thread.h"
#include "string.h"
#include <math.h>
#include <stdlib.h>
using namespace std;
int main(int argc, char *argv[])
{

    // Initialize variables
    int j, i;
    int size; // also known as n
    int k;
    int stage, gap;
    /*
    thread* test[3];
    for(int i = 0; i < 3; i++){
        test[i] = new thread(i);
        test[i]->Begin()
    }
    */
    // ( -> ) exists to access the members of the structure or the unions using pointers.
    // ( . )  is used to normally access members of a structure or union.

    cin >> size;
    int initialArray[size];
    for (int i = 0; i < size; i++)
    {
        cin >> initialArray[i];
    }
    cout << size << endl;
    for (int i = 0; i < size; i++)
    {
        cout << initialArray[i] << " ";
    }
    cout << endl;

    k = log2(size);
    cout << "k = " << k << endl;
    int prefixSumArray[k][size];
    //intialize the first row on prefixSumArray
    for (int i = 0; i < size; i++)
    {
        prefixSumArray[0][i] = initialArray[i];
        cout << prefixSumArray[0][i] << " ";
    }
    cout << endl;
    //prefix calculation
    for (stage = 1, gap = 1; stage <= k; stage++, gap *= 2)
    {
        for (i = 0; i < size; i++)
        {

            if (i - gap < 0)
                prefixSumArray[stage][i] = prefixSumArray[stage-1][i];
            else
                prefixSumArray[stage][i] = prefixSumArray[stage - 1][i] + prefixSumArray[stage - 1][i-gap];
            cout << prefixSumArray[stage][i] << " ";
        }
        cout << endl;
    }

    //Example run of thread
    thread firstThread(size);
    firstThread.Begin();
    //Move all the above calculation into thread.cpp
    firstThread.Join();    

    cout << "After thread.cpp, the prefixSumArray is:" << endl;
    for (j = 0; j <= k; j++)
    {
        for (i = 0; i < size; i++)
        {
            //cout << prefixSumArray[j][i] << " ";
        }
       //cout << endl;
    }

    Exit();

    return 0;
}