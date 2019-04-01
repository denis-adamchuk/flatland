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
    unsigned long aliveCells = 0;

    for (size_t j = 0; j < _currentGeneration._dimensions._height; ++j)
    {
        for (size_t i = 0; i < _currentGeneration._dimensions._width; ++i)
        {
            if (isAliveCell(i, j))
                ++aliveCells;
        }
    }

    _lastStatSnapshot["AliveCells"].Set(aliveCells);
    _lastStatSnapshot["AliveCellsDelta"].Set(static_cast<long>(aliveCells));
    _lastStatSnapshot["Generation"].Set(static_cast<unsigned long>(0));
    _lastStatSnapshot["LoopDetected"].Set(false);
}

bool SimpleFlatland::Run()
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

    auto prevAliveCells = _lastStatSnapshot["AliveCells"].Get<unsigned long>();
    _lastStatSnapshot["AliveCellsDelta"].Set(static_cast<long>(aliveCells - prevAliveCells));
    _lastStatSnapshot["AliveCells"].Set(aliveCells);
    auto prevGeneration = _lastStatSnapshot["Generation"].Get<unsigned long>();
    _lastStatSnapshot["Generation"].Set(prevGeneration + 1);

    if (!_lastStatSnapshot["LoopDetected"].Get<bool>())
    {
        _cycleFinder.Apply(aliveCells);
        if (_cycleFinder.HasCycle())
            _lastStatSnapshot["LoopDetected"].Set(true);
    }

    return aliveCells > 0;
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

const StatisticsMap& SimpleFlatland::GetStatistics() const
{
    return _lastStatSnapshot;
}

bool SimpleFlatland::isAliveCell(size_t i, size_t j) const
{
    return ReadCell(_currentGeneration, i, j);
}

}

}
