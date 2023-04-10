// ------------------------------------------------
// NAME : Chanpech Hoeng ID: choeng
// DUE DATE : 04/6/2023
// PROGRAM ASSIGNMENT #4
// FILE NAME : thread-support.cpp
// PROGRAM PURPOSE : Contains all supporting functions such as
// wake_mother_up(int), search_feeding_pots(int), 
// ready_to_eat(int), finish_eating(int), goto_sleep() and
// food_ready(int)
// -------------------------------------------------

Semaphore Blocked("Lock", num_child);       // Handles waiting for food to be available
Semaphore MotherSleep("MotherLock", 0);     // Handles the mother thread state
Semaphore Mutex("MyLock", 1);               // Handles the critical section 
Semaphore Eating("Lock", 0);                // Handles the baby eagle eat and finishes state

int numBabyWaiting = 0;                     // Number of eagle threads to be free when food is ready


// -------------------------------------------------------
// FUNCTION thread::ready_to_eat()
// DESCRIPTION:
//      It is in a critical section and Mother thread will be signal.
// PARAMETER USAGE : int babyNum
// FUNCTION CALLED : search_feeding_pots(babyNum)
void wake_mother_up(int babyNum)
{
        char buffer[255];
        MotherSleep.Signal();
        sprintf(buffer, "Mother eagle is awoke by baby eagle %d and starts preparing food.\n", babyNum);
        write(1, buffer, strlen(buffer));
}

// -------------------------------------------------------
// FUNCTION thread::search_feeding_pots()
// DESCRIPTION:
//      Baby eagle will call this method to search for available feeding pots. 
//      If none available one baby thread will wake_mother_up() and enter a waiting state until food is ready.
//      When ready it will be release from waiting state and it will search_feeding_pots again until it find foods.
// PARAMETER USAGE : int babyNum
// FUNCTION CALLED : search_feeding_pots(babyNum)
int search_feeding_pots(int babyNum, int limit)
{
    char buffer[255];

    for (int i = 0; i < totalPots; i++)
    {
        Mutex.Wait();
        if (feedingPots[i] == 1)
        {
            char buf[255];
            sprintf(buf, "%*sBaby eagle %d is eating using feeding pot %d.\n", -babyNum, "", babyNum, i + 1);
            write(1, buf, strlen(buf));

            feedingPots[i] = 0; // Eating thus no longer available
            Mutex.Signal();
            Eating.Wait();
            return i; // return INDEX
        }
        Mutex.Signal();
    }

    Mutex.Wait();
    if(counter >= limit){
        counter++;
        Mutex.Signal();
        return 0;
    }
    if (motherAwake == false)
    {
        char buf[255];
        sprintf(buf, "%*sBaby eagle %d sees all feeding pots are empty and wakes up the mother.\n", -babyNum, "", babyNum);
        write(1, buf, strlen(buf));
        motherAwake = true;
        numBabyWaiting++;
        wake_mother_up(babyNum);
        Mutex.Signal();
        Blocked.Wait();
    }
    else
    {
        numBabyWaiting++;
        Mutex.Signal();
        Blocked.Wait();
    }

    search_feeding_pots(babyNum, limit);
    return -1;
}
// -------------------------------------------------------
// FUNCTION thread::ready_to_eat()
// DESCRIPTION:
//      blocks the caller, a baby eagle, if all feeding pots are empty. One
//      and only one baby eagle who finds out all feeding pots being empty should wake up the
//      mother eagle. This function returns only if there is a feeding pot with food available to
//      this baby eagle
// PARAMETER USAGE : int babyNum
// FUNCTION CALLED : search_feeding_pots(babyNum)
// --------------------------------------------------------
void ready_to_eat(int babyNum, int limit)
{
    char buffer[255];
    Eating.Signal();
    sprintf(buffer, "%*sBaby eagle %d ready to eat.\n", -babyNum, "", babyNum);
    write(1, buffer, strlen(buffer));
    // Search for feeding pot with food available 1 = Available, 0 = N/A
    int eating = search_feeding_pots(babyNum, limit);
}
// -------------------------------------------------------
// FUNCTION thread::finish_eating()
// DESCRIPTION:
//      should be called when a baby eagle finishes eating
// PARAMETER USAGE : int babyNum
// FUNCTION CALLED : Signal()
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
// FUNCTION CALLED : Wait()
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
// PARAMETER USAGE : int cycle
// FUNCTION CALLED : Signal()
// --------------------------------------------------------
void food_ready(int cycle)
{
    char buffer[255];
    counter++;
    // Putting foods into all feedingPots
    for (int i = 0; i < totalPots; i++)
    {
        feedingPots[i] = 1;
    }
    sprintf(buffer, "Mother eagle says \"Feeding (%d)\"\n", counter);
    write(1, buffer, strlen(buffer));

    //Signal all the babies that waited for foods to be available
    for (int i = 0; i < numBabyWaiting; i++) 
    {
        Blocked.Signal();
    }
}