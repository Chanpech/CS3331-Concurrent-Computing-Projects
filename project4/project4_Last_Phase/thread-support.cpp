// ------------------------------------------------
// NAME : Chanpech Hoeng ID: choeng
// DUE DATE : 03/17/2023
// PROGRAM ASSIGNMENT #3
// FILE NAME : thread.cpp
// PROGRAM PURPOSE :
// The prefix sum program takes in input files of size n and an array of integers.
// To produce the sequence the program utilizes ThreadMentor threads to handle it computation.
// Although the program ran at O(n log(n)), it uses threads to computate concurrently.
// -------------------------------------------------

Semaphore Block("Lock", 0);
Semaphore MotherSleep("MotherLock", 0);
Semaphore Mutex("MyLock", 1);
Semaphore Eating("Lock", 0);

void wake_mother_up(int babyNum)
{
    char buffer[255];
    // wake up mother
    MotherSleep.Signal();
    sprintf(buffer, "Mother eagle is awoke by baby eagle %d and starts preparing food.\n", babyNum);
    write(1, buffer, strlen(buffer));

    motherAwake = true;
    // Mutex.Signal();
}

int search_feeding_pots(int babyNum)
{
    char buffer[255];
    // Mutex.Wait();
    for (int i = 0; i < totalPots; i++)
    {
        Mutex.Wait();
        if (feedingPots[i] == 1)
        {
            char buf[255];
            sprintf(buf, "%*sBaby eagle %d is eating using feeding pot %d.\n", -babyNum, "", babyNum, i + 1);
            write(1, buf, strlen(buf));
            feedingPots[i] = 0; // Set feeding pot to zero
            Mutex.Signal();
            Eating.Wait();
            return i; // return INDEX
        }
        Mutex.Signal();
    }
    Mutex.Wait();
    if (motherAwake == false)
    {
        wake_mother_up(babyNum);
    }
    Mutex.Signal();
    search_feeding_pots(babyNum);
    return -1;
}
// -------------------------------------------------------
// FUNCTION thread::ready_to_eat()
// DESCRIPTION:
//      blocks the caller, a baby eagle, if all feeding pots are empty. One
//      and only one baby eagle who finds out all feeding pots being empty should wake up the
//      mother eagle. This function returns only if there is a feeding pot with food available to
//      this baby eagle
// PARAMETER USAGE : None Used
// FUNCTION CALLED : None Used
// --------------------------------------------------------
void ready_to_eat(int babyNum)
{
    char buffer[255];
    Eating.Signal();
    sprintf(buffer, "%*sBaby eagle %d ready to eat.\n", -babyNum, "", babyNum);
    write(1, buffer, strlen(buffer));
    // Search for feeding pot with food available 1 = Available, 0 = N/A
    int eating = search_feeding_pots(babyNum);

    /*
        if (eating < 0)
        {
            char buff[255];
            Mutex.Wait();
            sprintf(buff, "%*sBaby eagle %d sees all feeding pots are empty and wakes up the mother.\n", -babyNum, "", babyNum);
            write(1, buff, strlen(buff));
            wake_mother_up(babyNum);
            Mutex.Signal();
            search_feeding_pots(babyNum);
        }*/
}
// -------------------------------------------------------
// FUNCTION thread::finish_eating()
// DESCRIPTION:
//      should be called when a baby eagle finishes eating
// PARAMETER USAGE : None Used
// FUNCTION CALLED : None Used
// --------------------------------------------------------
void finish_eating(int babyNum)
{
    char buffer[255];
    sprintf(buffer, "%*sBaby eagle %d finishes eating.\n", -babyNum, "", babyNum);
    write(1, buffer, strlen(buffer));
    Eating.Signal(); // return block
}
// -------------------------------------------------------
// FUNCTION thread::goto_sleep()
// DESCRIPTION:
//      Only called by the mother eagle when she wants to take a nap
// PARAMETER USAGE : None Used
// FUNCTION CALLED : None Used
// --------------------------------------------------------
void goto_sleep()
{
    char buffer[255];
    sprintf(buffer, "Mother eagle takes a nap.\n");
    write(1, buffer, strlen(buffer));
    MotherSleep.Wait(); // Mother take a nap
    motherAwake = false;
}
// -------------------------------------------------------
// FUNCTION thread::food_ready ()
// DESCRIPTION:
//      is called when the mother eagle has finished adding food in all m
//      feeding pots
// PARAMETER USAGE : None Used
// FUNCTION CALLED : None Used
// --------------------------------------------------------
void food_ready(int cycle)
{
    char buffer[255];
    counter++;
    for (int i = 0; i < totalPots; i++)
    {
        feedingPots[i] = 1;
    }
    sprintf(buffer, "Mother eagle says \"Feeding (%d)\"\n", counter);
    write(1, buffer, strlen(buffer));
}