#pragma once

#include <deque>
#include <vector>

namespace flatland
{

namespace lib
{

// Finds loops in infinite sequence of numbers
class CycleFinder
{
public:
    // User has to specify:
    // - an upper limit for a length of a cycle to be found
    // - a number of confirmations for a found cycle
    CycleFinder(unsigned long maxCycleLength, unsigned long confirmationCount);

    // Applies one more number to the finder
    void Apply(unsigned long item);

    // Checks if a confirmed cycle already found
    bool HasCycle();

private:
    std::deque<unsigned long> _window;
    const unsigned long _maxCycleLength;
    const unsigned long _confirmationCount;
    const unsigned long _windowSize;

    std::vector<unsigned long> _cycle;
    unsigned long _cycles;
};

}

}
