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
void wordleHelper(
    std::string& in,
    std::map<char, int>& float_map,
    const std::set<std::string>& dict, 
    unsigned int index,
    std::set<std::string>& solution
);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    std::set<std::string> solution;     // to return
    std::string in_copy = in;           // copy to be modified
    std::map<char, int> float_map;      // hold all in letters

    for (char c : floating)
    {
        float_map[c]++;
    }

    wordleHelper(in_copy, float_map, dict, 0, solution);
    return solution;
}


// Define any helper functions here
void wordleHelper(
    std::string& in,
    std::map<char, int>& float_map,
    const std::set<std::string>& dict, 
    unsigned index,
    std::set<std::string>& solution
)
{
    // BASE CASE
    if (index == in.size())
    {
        // all floating letters were used
        if (!float_map.empty())
        {
            return;
        }
        // word exists in dictionary
        if (dict.count(in))
        {
            solution.insert(in);
        }
        return;
    }

    // SKIP: if letter is already filled in
    if (in[index] != '-')
    {
        wordleHelper(in, float_map, dict, index + 1, solution);
    }
    // RECURSE: 
    else
    {
        // LIMITATIONS: '-' must be more then number of floating letters left
        int dashes = 0;
        int floatingChars = 0;
        for (char c : in)
        {
            if (c == '-')
            {
                dashes++;
            }
        }
        std::map<char, int>::iterator it;
        for (it = float_map.begin(); it != float_map.end(); ++it)
        {
            floatingChars += it->second;
        }
        if (floatingChars > dashes)
        {
            return;
        }
        // RECURSE: all combinations
        for (char c = 'a'; c <= 'z'; c++)
        {
            in[index] = c;
            std::map<char, int> updated_float_map = float_map;
            // If we use a floating letter, remove from map
            if (updated_float_map.count(c))
            {
                updated_float_map[c]--;
                if (updated_float_map[c] == 0)
                {
                    updated_float_map.erase(c);
                }
            }
            wordleHelper(in, updated_float_map, dict, index + 1, solution);
        }
        in[index] = '-'; // backtrack
    }
}