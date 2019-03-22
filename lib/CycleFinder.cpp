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
    if (_window.size() < _windowSize)
    {
        _window.push_back(item);

        if (_window.size() < _windowSize)
            return;
    }

    long shift = 1;
    for (size_t iCycleLengthToCheck = _maxCycleLength; iCycleLengthToCheck > 0; --iCycleLengthToCheck)
    {
        bool found = true;
        for (size_t i = 0; i < iCycleLengthToCheck; ++i)
        {
            if (_window[i] != _window[i + iCycleLengthToCheck])
            {
                found = false;
                break;
            }
        }
        if (found)
        {
            if (_cycle.size() != iCycleLengthToCheck)
                _cycles = 0;
            else
            {
                bool same = true;
                for (size_t i = 0; i < iCycleLengthToCheck; ++i)
                {
                    if (_window[i] != _cycle[i])
                        same = false;
                }
                if (!same)
                    _cycles = 0;
            }

            ++_cycles;
            shift = static_cast<long>(iCycleLengthToCheck);
            _cycle.assign(_window.begin(), _window.begin() + shift);
            break;
        }
    }

    if (_cycles >= _confirmationCount)
        return;

    for (size_t iSkip = 0; iSkip < static_cast<size_t>(shift); ++iSkip)
        _window.pop_front();
}

bool CycleFinder::HasCycle()
{
    return _cycles >= _confirmationCount;
}

}

}
