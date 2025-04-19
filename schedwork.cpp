#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    vector<size_t>& shiftCount,
    unsigned int day, 
    unsigned int workers
);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below
    // avail = rows x col (days x workers)
    size_t numDays = avail.size();
    size_t numWorkers = avail[0].size();

    // sched rows x col (days x dailyNeed)
    for (unsigned int i = 0; i < numDays; i++)
    {
        vector<Worker_T> days;
        sched.push_back(days);
    }
    vector<size_t> shiftCount(numWorkers, 0);
    return scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftCount, 0, 0);

}


bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    vector<size_t>& shiftCount,
    unsigned int day, 
    unsigned int workers
) 
{
    // BASE CASE: schedule completed
    if (day == avail.size())
    {
        return true;
    }
    // ITERATE: through all options
    for(int i = 0; i < (int) avail[0].size(); i++)
    {
        /* IS VALID CONDITIONS:
         * 1. avalibility         
         * 2. worker does not work more than maxShifts
         * 3. there is at least shiftCount workers working per day */

        // CONDITIONS 1 and 2
        if (avail[day][i] && shiftCount[i] < maxShifts) 
        {
            sched[day].push_back(i);
            shiftCount[i]++;

            // CONDITION 3: only move onto the next day if current day has enough workers 
            if (workers + 1 == dailyNeed) 
            {
                return scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftCount, day + 1, 0);
            }
            // All options to get to the end of the day
            if (scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftCount, day, workers + 1))
            {
                return true;
            }

            // BACKTRACK
            sched[day].pop_back();
            shiftCount[i]--;
        }
    }
    return false;
}

