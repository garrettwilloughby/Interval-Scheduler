//
//  ECSimTaskScheduler2.cpp
//
//
//

#include <vector>
#include <iostream>
using namespace std;

#include "ECSimTaskScheduler2.h"
#include "ECSimTask.h"

//***********************************************************
// get by task wait time
ECSimLWTFTaskScheduler ::ECSimLWTFTaskScheduler()
{
}

// Choose from a list of tasks that are ready to run
ECSimTask *ECSimLWTFTaskScheduler ::ChooseTaskToSchedule(const std::vector<ECSimTask *> &listReadyTasks) const
{

    // loop through list
    if (listReadyTasks.size() > 0)
    {

        ECSimTask *Next = listReadyTasks[0];

        for (auto task : listReadyTasks)
        {
            if (task->GetTotWaitTime() > Next->GetTotWaitTime())
            {

                Next = task;
            }
        }
        return Next;
    }

    else
    {
        return NULL;
    }
}

//***********************************************************
// get by task priority
ECSimPriorityScheduler ::ECSimPriorityScheduler()
{
}

// Choose from a list of tasks that are ready to run
ECSimTask *ECSimPriorityScheduler ::ChooseTaskToSchedule(const std::vector<ECSimTask *> &listReadyTasks) const
{

    // loop through list
    if (listReadyTasks.size() > 0)
    {

        ECSimTask *Next = listReadyTasks[0];

        for (auto task : listReadyTasks)
        {
            if (task->GetPriority() < Next->GetPriority())
            {

                Next = task;
            }
        }
        return Next;
    }

    else
    {
        return NULL;
    }
}

//***********************************************************
// get by least run?

ECSimRoundRobinTaskScheduler ::ECSimRoundRobinTaskScheduler()
{
}

// choose from list
ECSimTask *ECSimRoundRobinTaskScheduler ::ChooseTaskToSchedule(const std::vector<ECSimTask *> &listReadyTasks) const
{

    // loop through list
    if (listReadyTasks.size() > 0)
    {

        ECSimTask *Next = listReadyTasks[0];

        for (auto task : listReadyTasks)
        {

            if (task->GetTotRunTime() < Next->GetTotRunTime())
            {
                Next = task;
            }
        }
        return Next;
    }

    else
    {
        return nullptr;
    }
}
