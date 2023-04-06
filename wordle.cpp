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
std::set<std::string> fillAllLetters(const std::string& s, const std::string& floating, int pos);
std::set<std::string> removeBadWords(const std::set<std::string>& in, const std::set<std::string>& dict);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
  std::set<std::string> correctWords = fillAllLetters(in, floating, 0);
  return removeBadWords(correctWords, dict);
}

// Define any helper functions here

std::set<std::string> fillAllLetters(const std::string& s, const std::string& floating, int pos)
{
  std::set<std::string> currSet{};
  if (s.empty() || pos >= s.size())
  {
    currSet.insert(s);
    return currSet;
  }
  //(s.substr(0, pos) + letter + s.substr(pos))
  if (s[pos] != '-')
  {
    return fillAllLetters(s, floating, pos + 1);
  }
  /*
  size_t i = 0;
  for (char letter : floating)
    if (s.find(letter) != s.npos)
      i++;
  */
  size_t numBlanks = 0;
  size_t blankPos = s.find_first_of('-');
  while (blankPos != s.npos)
  {
    blankPos = s.find_first_of('-', blankPos + 1);
    numBlanks++;
  }
  if (floating.size() >= numBlanks)
  {
    for (char letter : floating)
    {
      auto i = floating.find(letter);
      std::set<std::string> subSet;
      if (i != floating.npos)
        subSet = fillAllLetters(s.substr(0, pos) + letter + s.substr(pos + 1), floating.substr(0, i) + floating.substr(i + 1), pos + 1);
      else
        subSet = fillAllLetters(s.substr(0, pos) + letter + s.substr(pos + 1), floating, pos + 1);
      currSet.insert(subSet.begin(), subSet.end());
    }
  }
  else
  {
    for (char letter = 'a'; letter <= 'z'; letter++)
    {
      auto i = floating.find(letter);
      std::set<std::string> subSet;
      if (i != floating.npos)
        subSet = fillAllLetters(s.substr(0, pos) + letter + s.substr(pos + 1), floating.substr(0, i) + floating.substr(i + 1), pos + 1);
      else
        subSet = fillAllLetters(s.substr(0, pos) + letter + s.substr(pos + 1), floating, pos + 1);
      currSet.insert(subSet.begin(), subSet.end());
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