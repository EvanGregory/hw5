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
  const size_t maxShifts, const DailySchedule& workerCombos,
  DailySchedule sched, const size_t pos);

Worker_T findWorker(const AvailabilityMatrix& avail, size_t day, size_t& pos);

void workerCombinations(std::vector<Worker_T> arr, int len, int startPosition, std::vector<Worker_T>& combo, DailySchedule& result);

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
    vector<Worker_T> combo;
    vector<Worker_T> workerIDs;
    DailySchedule workerCombos;
    //initialize combo
    for (int i = 0; i < dailyNeed; i++)
    {
      combo.push_back(INVALID_ID);
    }
    //initialize ids
    for (int i = 0; i < avail[0].size(); i++)
    {
      workerIDs.push_back(i);
    }

    //generate all possible combinations
    workerCombinations(workerIDs, dailyNeed, 0, combo, workerCombos);
    pair<bool, DailySchedule> final = recurseFillSched(avail, dailyNeed, maxShifts, workerCombos, sched, 0);
    if (!final.first)
      return false;
    sched = final.second;
    return true;
}

std::pair<bool, DailySchedule> recurseFillSched(const AvailabilityMatrix& avail,
  const size_t dailyNeed,
  const size_t maxShifts, 
  const DailySchedule& workerCombos,
  DailySchedule sched, const size_t pos)
{

  //checking if sched fails
  {
    //checks if previous day uses only available workers
    if (pos > 0)
      for (Worker_T worker : sched[pos - 1])
        if (!avail[pos - 1][worker])
          return make_pair(false, sched);

    //checks if any worker is overworked
    vector<int> daysWorked;
    for (bool t : avail[0])
      daysWorked.push_back(0);

    for (int day = 0; day < pos; day++)
    {
      for (int i = 0; i < dailyNeed; i++)
      {
        Worker_T worker = sched[day][i];
        daysWorked[worker]++;
        if (daysWorked[worker] > maxShifts)
          return make_pair(false, sched);
      }
    }
    //could add more precise backtracing checks here, look ahead to see if the current state is impossible
  }

  //reached the end, return true
  if (avail.size() <= pos)
  {
    return make_pair(true, sched);
  }

  for (vector<Worker_T> attempt : workerCombos)
  {
    DailySchedule tempSched = sched;
    tempSched.push_back(attempt);
    std::pair<bool, DailySchedule> pairAttempt = recurseFillSched(avail, dailyNeed, maxShifts, workerCombos, tempSched, pos + 1);
    if (pairAttempt.first)
      return pairAttempt;
  }
  return make_pair(false, sched);

  //vector<vector<Worker_T>> attemptList;
  //while(true)
  //{
  //  size_t workerPos = 0;
  //  DailySchedule tempSched = sched;
  //  //pushback a worker dailyNeed times
  //  for (size_t num = 1; num <= dailyNeed; num++)
  //  {
  //    
  //    vector<Worker_T> superTemp;
  //    Worker_T currWorker;
  //    //if we have already tried that worker combination, try again
  //    do { 
  //      superTemp = tempSched[pos];
  //      currWorker = findWorker(avail, pos, workerPos); 
  //      superTemp.push_back(currWorker);
  //    }
  //    while (std::find(attemptList.begin(), attemptList.end(), superTemp) != attemptList.end());
  //    if (currWorker == INVALID_ID)
  //    {
  //      //find last available worker
  //      return make_pair(false, sched);
  //    }
  //    tempSched[pos].push_back(currWorker);

  //  }
  //  std::pair<bool, DailySchedule> attempt = recurseFillSched(avail, dailyNeed, maxShifts, tempSched, pos + 1);
  //  if (attempt.first)
  //    return attempt;
  //  attemptList.push_back(attempt.second[pos]);
  //}
  //return make_pair(false, sched);

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



// ex. combinations2(arr, 3, 0, new String[3]);

void workerCombinations(vector<Worker_T> arr, int len, int startPosition, vector<Worker_T>& combo, DailySchedule& result) 
{
  if (len == 0)
  {
    result.push_back(combo);
    return;
  }

  for (int i = startPosition; i <= arr.size() - len; i++) 
  {

    combo[combo.size() - len] = arr[i];
    workerCombinations(arr, len - 1, i + 1, combo, result);
  }
}