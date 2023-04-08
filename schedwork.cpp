#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <numeric>
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
    combo.resize(dailyNeed, INVALID_ID);

    //initialize ids
    workerIDs.resize(avail[0].size())
    std::iota(workerIDs.begin(), workerIDs.end(), 0);

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
    daysWorked.resize(avail[0].size());

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
