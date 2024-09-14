//
//  ECSimTask3.cpp
//
//

#include "ECSimTask3.h"
#include <iostream>
using namespace std;

//***********************************************************
// Basic task
//***********************************************************

//***********************************************************
// Interval task: a single interval.
// YW: you shouldn't need to change this class!

ECSimIntervalTask ::ECSimIntervalTask(const std::string &tidIn, int tmStartIn, int tmEndIn) : tid(tidIn), tmStart(tmStartIn), tmEnd(tmEndIn), tmTotWait(0), tmTotRun(0)
{
}

// Is task ready to run at certain time? tick: the current clock time (in simulation unit)
bool ECSimIntervalTask ::IsReadyToRun(int tick) const
{
    return tick >= tmStart && tick <= tmEnd;
}

// Is task complete at certain time? If so, scheduler may remove it from the list. tick the current clock time (in simulation unit)
bool ECSimIntervalTask ::IsFinished(int tick) const
{
    return tick > tmEnd;
}

//***********************************************************
// Consecutive task: a task that can early abort

ECSimConsecutiveTask::ECSimConsecutiveTask(ECSimTask *pTask) : pTask(pTask), start(false), interrupted(false)
{
}

bool ECSimConsecutiveTask ::IsFinished(int tick) const
{
    // std::cout << "IsFinished" << std::endl;
    return pTask->IsFinished(tick) || interrupted;
}

bool ECSimConsecutiveTask::IsReadyToRun(int tick) const
{
    // std::cout << "IsReady" << std::endl;
    return pTask->IsReadyToRun(tick) && !interrupted;
}

void ECSimConsecutiveTask::Wait(int tick, int duration)
{
    // std::cout << "Wait" << std::endl;
    //  use flag?
    if (start)
    {
        // use is aborted here instead??
        interrupted = true;
    }
    // call original wait
    pTask->Wait(tick, duration);
}

void ECSimConsecutiveTask::Run(int tick, int duration)
{
    // std::cout << "Run" << std::endl;
    if (!interrupted)
    {
        // Execute the task for the given duration
        start = true;
        // call original run
        pTask->Run(tick, duration);
    }
}
//***********************************************************
// Periodic task: a task that can early abort

ECSimPeriodicTask::ECSimPeriodicTask(ECSimTask *pTask, int lenSleep) : pTask(pTask), lenSleep(lenSleep), tmStart(-1), tmEnd(-1), runLen(-1)
{
}

int ECSimPeriodicTask::calculateRunLength(int tick)
{
    return ((tick - tmStart) % (lenSleep + tmEnd - tmStart) + tmStart);
}

bool ECSimPeriodicTask::IsReadyToRun(int tick) const
{

    if (tmStart == -1 && pTask->IsReadyToRun(tick))
    {
        tmStart = tick;
    }

    if (tmEnd == -1 && (pTask->IsFinished(tick)))
    {
        tmEnd = tick;
    }
    if (tmEnd < 0)
    {
        return pTask->IsReadyToRun(tick);
    }
    return pTask->IsReadyToRun((tick - tmStart) % (lenSleep + tmEnd - tmStart) + tmStart);
}

void ECSimPeriodicTask::Wait(int tick, int duration)
{
    // call original wait
    // std::cout << "ECSimPeriodicTask::Wait" << std::endl;
    pTask->Wait(tick, duration);
}

void ECSimPeriodicTask::Run(int tick, int duration)
{
    // std::cout << "ECSimPeriodicTask::Run" << std::endl;
    pTask->Run(tick, duration);
}

ECSimStartDeadlineTask ::ECSimStartDeadlineTask(ECSimTask *pTask, int tmStartDeadlineIn) : pTask(pTask), tmStartDeadline(tmStartDeadlineIn)
{
}

// your code here

//***********************************************************
// Task must end by some fixed time click: this is useful e.g. when a task is periodic

ECSimEndDeadlineTask ::ECSimEndDeadlineTask(ECSimTask *pTask, int tmEndDeadlineIn) : pTask(pTask), tmEndDeadline(tmEndDeadlineIn)
{
}

//***********************************************************
// Composite task: contain multiple sub-tasks

ECSimCompositeTask ::ECSimCompositeTask(const std::string &tidIn) : tidIn(tidIn), tmTotRun(0), tmTotWait(0)
{
}

std::string ECSimCompositeTask::GetId() const { return tidIn; }
void ECSimCompositeTask::AddSubtask(ECSimTask *pt)
{
    tasklist.push_back(pt);
}

bool ECSimCompositeTask::IsReadyToRun(int tick) const
{
    for (const auto &i : tasklist)
    {
        if (i->IsReadyToRun(tick))
        {
            return true;
        }
    }
    return false;
}

bool ECSimCompositeTask::IsFinished(int tick) const
{
    for (const auto &i : tasklist)
    {
        if (!i->IsFinished(tick))
        {
            return false;
        }
    }
    return true;
}

// Is task early abort? There can be various reasons for abort: e.g., missed deadline

// Run the task for some duration (usually 1, but can be more) starting from time tick
void ECSimCompositeTask::Run(int tick, int duration)
{
    tmTotRun += duration;
    for (auto &i : tasklist)
    {
        if (i->IsReadyToRun(tick))
        {
            i->Run(tick, duration);
            return;
        }
    }
}

void ECSimCompositeTask::Wait(int tick, int duration)
{
    tmTotWait += duration;
    for (auto &i : tasklist)
    {
        if (i->IsReadyToRun(tick))
        {
            i->Wait(tick, duration);
        }
    }
}

// How much total time does the task has to wait to get its turn so far?
// int ECSimCompositeTask::GetTotWaitTime() const
// {
//     for (auto &i : tasklist)
//     {
//         i->GetTotWaitTime();
//     }
// }

// // Get total run-time (so far)
// int ECSimCompositeTask::GetTotRunTime() const
// {
//     for (auto &i : tasklist)
//     {
//         i->GetTotRunTime();
//     }
// }

bool ECSimCompositeTask::IsAborted(int tick) const
{
    for (auto &i : tasklist)
    {
        if (i->IsAborted(tick))
        {
            return true;
        }
    }
    return false;
}

// your code here
