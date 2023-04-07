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
std::pair<bool, DailySchedule> recurseFillSched(const AvailabilityMatrix& avail,
  const size_t dailyNeed,
  const size_t maxShifts,
  DailySchedule sched, const size_t pos);

Worker_T findWorker(const AvailabilityMatrix& avail, size_t day, size_t& pos);
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
    pair<bool, DailySchedule> final = recurseFillSched(avail, 2, maxShifts, sched, 0);
    if (!final.first)
      return false;
    sched = final.second;
    return true;
}

std::pair<bool, DailySchedule> recurseFillSched(const AvailabilityMatrix& avail,
  const size_t dailyNeed,
  const size_t maxShifts,
  DailySchedule sched, const size_t pos)
{
  //if pos is at the end, return true, return sched
  if (avail.size() >= pos)
    return make_pair(true, sched);

  {//make sure sched has room on the next line
    vector<Worker_T> t;
    sched.push_back(t);
  }

  //if current sched will not work, return false, dont care about state of return sched
  bool schedWorks = true;
  vector<int> daysWorked;
  for (bool t : avail[0])
    daysWorked.push_back(0);

  for (int day = 0; day <= pos; day++)
  {
    for (int i = 0; i < sched[day].size(); i++)
    {
      Worker_T worker = sched[day][i];
      daysWorked[worker]++;
      if (daysWorked[worker] > maxShifts)
        schedWorks = false;
    }
  }
  //could add more precise backtracing checks here, look ahead to see if the current state is impossible
  if (!schedWorks)
    return make_pair(false, sched);


  while(true)
  {
    size_t workerPos = 0;
    DailySchedule tempSched = sched;
    //pushback a worker dailyNeed times
    for (size_t num = 1; num <= dailyNeed; num++)
    {
      Worker_T currWorker = findWorker(avail, pos, workerPos);
      if (currWorker == INVALID_ID)
        return make_pair(false, sched);
      tempSched[pos].push_back(currWorker);
    }
    std::pair<bool, DailySchedule> attempt = recurseFillSched(avail, dailyNeed, maxShifts, tempSched, pos + 1);
    if (attempt.first)
      return attempt;
  }
  return make_pair(false, sched);

  //for each possible combination of available workers today, recursive call passing down sched with one more day filled in. pass pos + 1 for next day

  //for (int worker1 = 0; worker1 < avail[pos].size() - 1; worker1++)
  //  if (avail[pos][worker1])
  //    for (int worker2 = worker1 + 1; worker2 < avail[pos].size(); worker2++)
  //      if (avail[pos][worker2])
  //      {
  //        DailySchedule tempSched = sched;
  //        tempSched[pos].push_back(worker1);
  //        tempSched[pos].push_back(worker2);
  //        std::pair<bool, DailySchedule> attempt = recurseFillSched(avail, dailyNeed, maxShifts, tempSched, pos + 1);
  //        if (attempt.first)
  //          return attempt;
  //      }
  //return make_pair(false, sched);
  //  //if true, return true recursive call
  ////if false, return false, dont care about state of sched
}


Worker_T findWorker(const AvailabilityMatrix& avail, size_t day, size_t& pos)
{
  if (pos >= avail[day].size()) return INVALID_ID;

  for (Worker_T worker = pos; worker < avail[day].size(); worker++)
  {
    if (avail[day][worker]) {
      pos = worker + 1;
      return worker;
    }
  }

  return INVALID_ID;
}