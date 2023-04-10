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

//thread::thread(){}

void thread::ThreadFunc()
{
    Thread::ThreadFunc();

    Exit();
}