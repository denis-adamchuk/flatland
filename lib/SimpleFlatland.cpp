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
            if (IsCellAlive(i, j))
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
            size_t siblings = 0;
            for (int x = -1; x < 2; ++x)
            {
                for (int y = -1; y < 2; ++y)
                {
                    if (x == 0 && y == 0)
                        continue;
                    if (i == 0 && x < 0)
                        continue;
                    if (j == 0 && y < 0)
                        continue;
                    if (i == _currentGeneration._dimensions._width - 1 && x > 0)
                        continue;
                    if (j == _currentGeneration._dimensions._height - 1 && y > 0)
                        continue;

                    siblings += IsCellAlive(static_cast<size_t>(x) + i, static_cast<size_t>(y) + j);
                }
            }

            const auto current = IsCellAlive(i, j);
            if (current && (siblings < 2 || siblings > 3))
                WriteCell(nextGeneration, i, j, false);
            else if (!current && siblings == 3)
                WriteCell(nextGeneration, i, j, true);

            aliveCells += current;
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
    return ReadCell(_currentGeneration, i, j);
}

const StatisticsMap& SimpleFlatland::GetStatistics() const
{
    return _lastStatSnapshot;
}

}

}
