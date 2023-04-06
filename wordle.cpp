#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here


// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
  std::set<std::string> correctWords = fillAllLetters(in, floating);
  return removeBadWords(correctWords, dict);
}

// Define any helper functions here

std::set<std::string> fillAllLetters(const std::string& s, const std::string& floating, int pos = 0)
{
  std::set<std::string> currSet{};
  if (s.empty() || pos >= s.size())
    return currSet;
  //(s.substr(0, pos) + letter + s.substr(pos))
  if (s[pos] != '-')
  {
    return fillAllLetters(s, floating, pos + 1);
  }
  else
  {
    if (floating.size() >= s.size() - pos)
    {
      for (char letter : floating)
      {
        std::set<std::string> subSet = fillAllLetters(s.substr(0, pos) + letter + s.substr(pos), floating, pos + 1);
        currSet.insert(subSet.begin(), subSet.end());
      }
    }
    else
    {
      for (char letter = 'a'; letter <= 'z'; letter++)
      {
        std::set<std::string> subSet = fillAllLetters(s.substr(0, pos) + letter + s.substr(pos), floating, pos + 1);
        currSet.insert(subSet.begin(), subSet.end());
      }
    }
  }
  return currSet;
}

std::set<std::string> removeBadWords(const std::set<std::string>& in, const std::set<std::string>& dict)
{
  std::set<std::string> realWords;
  for (std::string word : in)
  {
    if (dict.find(word) != dict.end())
    {
      realWords.insert(word);
    }
  }
  return realWords;
}