#include "CycleFinder.h"

namespace flatland
{

namespace lib
{

CycleFinder::CycleFinder(unsigned long maxCycleLength, unsigned long confirmationCount)
    : _maxCycleLength(maxCycleLength)
    , _confirmationCount(confirmationCount)
    , _windowSize(2 * _maxCycleLength)
    , _cycles(0)
{
}

void CycleFinder::Apply(unsigned long item)
{
    // Collect a 'window' of elements first
    if (_window.size() < _windowSize)
    {
        _window.push_back(item);

        // Too early to check anything
        if (_window.size() < _windowSize)
            return;
    }

    // Tries to find cycles iterating from longest to shortest
    long shift = 1;
    for (size_t iCycleLengthToCheck = _maxCycleLength; iCycleLengthToCheck > 0; --iCycleLengthToCheck)
    {
        // Compares beginning of the window with elements shifted to the right for 'cycle' length
        bool found = true;
        for (size_t i = 0; i < iCycleLengthToCheck; ++i)
        {
            if (_window[i] != _window[i + iCycleLengthToCheck])
            {
                found = false;
                break;
            }
        }
        if (found) // There is a match within the window
        {
            // Check if we have a cycle of the same length as the found match in our storage
            if (_cycle.size() != iCycleLengthToCheck)
                _cycles = 0; // Length mismatches, forget stored cycle
            else
            {
                // Length matches, compare contents
                bool same = true;
                for (size_t i = 0; i < iCycleLengthToCheck; ++i)
                {
                    if (_window[i] != _cycle[i])
                        same = false;
                }
                if (!same)
                    _cycles = 0; // Content mismatches, forget stored cycle
            }

            // Let's consider a found match as a potential cycle, may be not even not for the first time
            ++_cycles;

            // Store potential cycle for later match with other cycles
            shift = static_cast<long>(iCycleLengthToCheck);
            _cycle.assign(_window.begin(), _window.begin() + shift);

            // Don't check shorter cycles, wait for new elements in the window
            break;
        }
    }

    if (_cycles >= _confirmationCount)
        return;

    // Stored potential cycle is no longer needed in the window
    for (size_t iSkip = 0; iSkip < static_cast<size_t>(shift); ++iSkip)
        _window.pop_front();
}

bool CycleFinder::HasCycle()
{
    return _cycles >= _confirmationCount;
}

}

}
