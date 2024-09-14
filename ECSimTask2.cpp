
//
//  ECSimTask2.cpp
//
//
//

#include "ECSimTask2.h"
#include <iostream>

//***********************************************************
// One-shot task: a task spans a single interval [a,b] of time; this task has soft deadline: it can only run within [a,b] but differently from hard interval: it can run partially as long as the time is within [a,b]

ECMultiIntervalsTask ::ECMultiIntervalsTask(const std::string &tid) : ECSimTask(tid)
{
}

// Is task ready to run at certain time? tick: the current clock time (in simulation unit)
void ECMultiIntervalsTask ::AddInterval(int a, int b)
{

    intervals.push_back(std::make_pair(a, b));
    // std::cout << intervals.size() << std::endl;
}

bool ECMultiIntervalsTask ::IsReadyToRun(int tick) const
{
    for (const auto &interval : intervals)
    {
        if (tick >= interval.first &&

            tick <= interval.second)
        {

            return true;
        }
    }
    return false;
}

bool ECMultiIntervalsTask ::IsFinished(int tick) const
{
    // beyond last interval, finish
    return tick > intervals.back().second;
}

void ECMultiIntervalsTask ::Wait(int tick, int duration)
{
    // std::cout << "Wait" << std::endl;
    int totalWait = totalWait;

    // std::cout << totalWait << std::endl;
    ECSimTask::Wait(tick, duration);
}

//***********************************************************

ECHardIntervalTask ::ECHardIntervalTask(const std::string &tid, int tmStart, int tmEnd)

    : ECSimTask(tid), tmStart(tmStart), tmEnd(tmEnd)
{
}

bool ECHardIntervalTask ::IsReadyToRun(int tick) const
{
    // needs to start at exact value of tmStart
    return (tick == tmStart);
}

bool ECHardIntervalTask ::IsFinished(int tick) const
{
    // needs to end at exact value of tmEnd or cant be ended
    return IsHard || tick >= tmEnd;
}

void ECHardIntervalTask::Wait(int tick, int duration)
{
    // std::cout << GetId() << "Wait: " << GetTotWaitTime() << std::endl;
    ECSimTask::Wait(tick, duration);
    IsHard = true;
}

//***********************************************************

ECConsecutiveIntervalTask::ECConsecutiveIntervalTask(const std::string &tid, int tmStart, int tmEnd)

    : ECSimTask(tid), tmStart(tmStart), tmEnd(tmEnd), interrupted(false), start(false)
{
}

// interrupted is a flag stored in class, start ensures we start and don't get kicked out right away
bool ECConsecutiveIntervalTask::IsReadyToRun(int tick) const
{
    return !interrupted && (tick >= tmStart) && (tick <= tmEnd);
}

bool ECConsecutiveIntervalTask::IsFinished(int tick) const
{
    return interrupted || tick > tmEnd;
}

void ECConsecutiveIntervalTask::Run(int tick, int duration)
{
    if (!interrupted)
    {

        // holy
        ECSimTask::Run(tick, duration);
        start = !start;
    }
}

void ECConsecutiveIntervalTask::Wait(int tick, int duration)
{

    if (start)
    {

        interrupted = true;
    }
    ECSimTask::Wait(tick, duration);
}

//***********************************************************

ECPeriodicTask::ECPeriodicTask(const std::string &tid, int tmStart, int runLen, int sleepLen)

    : ECSimTask(tid), tmStart(tmStart), runLen(runLen), sleepLen(sleepLen)
{
}

bool ECPeriodicTask::IsFinished(int tick) const
{
    // tick <= tmStart;
    return false;
}

void ECPeriodicTask ::Run(int tick, int duration)
{

    if (IsReadyToRun(tick))
    {
        ECSimTask::Run(tick, duration);
    }
}

bool ECPeriodicTask::IsReadyToRun(int tick) const
{
    // return tick <= tmStart;
    // funny
    bool 西瓜 = ((tick - tmStart) % (runLen + sleepLen) < runLen);
    bool 鸡蛋 = ((tick - tmStart) >= 0);
    return 西瓜 && 鸡蛋;
}
