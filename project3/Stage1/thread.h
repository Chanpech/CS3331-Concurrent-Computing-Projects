#include "ThreadClass.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

class thread : public Thread //thread is declared as a derived class of thread
{
    public:
        //thread();
    private:
        //All executable code must be in function ThreadFunc()
        //int n;
        void ThreadFunc();
};