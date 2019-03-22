#include "SimpleFlatland.h"
#include "Common.h"

#include <stdexcept>

namespace
{
    const size_t sc_maxCycleLength{ 10 };
    const size_t sc_cyclesConfirmationCount{ 5 };
}

namespace flatland
{

namespace lib
{

SimpleFlatland::SimpleFlatland(const SimpleCellMap& flatlandMap)
    : _currentGeneration(flatlandMap)
    , _cycleFinder(sc_maxCycleLength, sc_cyclesConfirmationCount)
{
    for (size_t j = 0; j < _currentGeneration._dimensions._height; ++j)
    {
        for (size_t i = 0; i < _currentGeneration._dimensions._width; ++i)
        {
            if (isAliveCell(i, j))
                ++_lastStatSnapshot._aliveCells;
        }
    }

    _lastStatSnapshot._aliveCellsDelta = static_cast<signed long>(_lastStatSnapshot._aliveCells);
    _lastStatSnapshot._generation = 0; // start
}

void SimpleFlatland::Run()
{
    using namespace std::placeholders;

    unsigned long aliveCells = 0;

    SimpleCellMap nextGeneration(_currentGeneration);
    for (size_t j = 0; j < _currentGeneration._dimensions._height; ++j)
    {
        for (size_t i = 0; i < _currentGeneration._dimensions._width; ++i)
        {
            const auto current = ReadCell(_currentGeneration, i, j);
            const auto siblings = CountSiblings(i, j,
                [this](size_t i, size_t j)
            {
                return isAliveCell(i, j);
            });

            if (current && (siblings < 2 || siblings > 3))
                WriteCell(nextGeneration, i, j, false);
            else if (!current && siblings == 3)
                WriteCell(nextGeneration, i, j, true);

            aliveCells += isAliveCell(i, j);
        }
    }

    _currentGeneration._map = std::move(nextGeneration._map);

    _lastStatSnapshot._aliveCellsDelta =
        static_cast<signed long>(aliveCells - _lastStatSnapshot._aliveCells);
    _lastStatSnapshot._aliveCells = aliveCells;
    ++_lastStatSnapshot._generation;
    if (!_lastStatSnapshot._loopDetected)
    {
        _cycleFinder.Apply(aliveCells);
        _lastStatSnapshot._loopDetected = _cycleFinder.HasCycle();
    }
}

size_t SimpleFlatland::Width() const
{
    return _currentGeneration._dimensions._width;
}

size_t SimpleFlatland::Height() const
{
    return _currentGeneration._dimensions._height;
}

bool SimpleFlatland::IsCellAlive(size_t i, size_t j) const
{
    if (i >= _currentGeneration._dimensions._width || j >= _currentGeneration._dimensions._height)
        throw std::range_error("Bad coordinate(s) passed");

    return ReadCell(_currentGeneration, i, j);
}

const Statistics& SimpleFlatland::GetStatistics() const
{
    return _lastStatSnapshot;
}

bool SimpleFlatland::isAliveCell(size_t i, size_t j) const
{
    return ReadCell(_currentGeneration, i, j);
}

}

}
