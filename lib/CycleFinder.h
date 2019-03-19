#pragma once

#include <deque>
#include <vector>

// Finds loops in infinite sequences
class CycleFinder
{
public:
    CycleFinder(unsigned long maxCycleLength, unsigned long confirmationCount);

    bool Apply(unsigned long item);

private:
    std::deque<unsigned long> _window;
    const unsigned long _maxCycleLength;
    const unsigned long _confirmationCount;
    const unsigned long _windowSize;

    std::vector<unsigned long> _cycle;
    unsigned long _cycles;
};
