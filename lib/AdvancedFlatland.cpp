#include "AdvancedFlatland.h"

#include <stdexcept>
#include <cassert>

namespace
{
    const size_t sc_maxCycleLength{ 10 };
    const size_t sc_cyclesConfirmationCount{ 5 };
}

namespace flatland
{

namespace lib
{

AdvancedFlatland::AdvancedFlatland(const AdvancedCellMap& flatlandMap, unsigned long maxAge,
    unsigned long maxReproductivityAge)
    : _currentGeneration(flatlandMap)
    , _maxAge(maxAge)
    , _maxReproductivityAge(maxReproductivityAge)
    , _cycleFinder(sc_maxCycleLength, sc_cyclesConfirmationCount)
{
    _lastStatSnapshot._aliveCells = 0;
    _lastStatSnapshot._reproductiveCells = 0;

    for (size_t j = 0; j < _currentGeneration._dimensions._height; ++j)
    {
        for (size_t i = 0; i < _currentGeneration._dimensions._width; ++i)
        {
            if (isAliveCell(i, j))
                ++_lastStatSnapshot._aliveCells;
            if (isReproductiveCell(i, j))
                ++_lastStatSnapshot._reproductiveCells;
        }
    }

    _lastStatSnapshot._aliveCellsDelta = static_cast<signed long>(_lastStatSnapshot._aliveCells);
    _lastStatSnapshot._reproductiveCellsDelta = static_cast<signed long>(_lastStatSnapshot._reproductiveCells);
    _lastStatSnapshot._generation = 0; // start
}

void AdvancedFlatland::Run()
{
    unsigned long aliveCells = 0;
    unsigned long reproductiveCells = 0;

    AdvancedCellMap nextGeneration(_currentGeneration);
    for (size_t j = 0; j < _currentGeneration._dimensions._height; ++j)
    {
        for (size_t i = 0; i < _currentGeneration._dimensions._width; ++i)
        {
            const auto current = ReadCell(_currentGeneration, i, j);
            const auto siblings =
                isAliveCell(i - 1, j - 1) + isAliveCell(i, j - 1) + isAliveCell(i + 1, j - 1)
              + isAliveCell(i - 1, j)                             + isAliveCell(i + 1, j)
              + isAliveCell(i - 1, j + 1) + isAliveCell(i, j + 1) + isAliveCell(i + 1, j + 1);
            const auto reproductiveSiblings =
                isReproductiveCell(i - 1, j - 1) + isReproductiveCell(i, j - 1) + isReproductiveCell(i + 1, j - 1)
              + isReproductiveCell(i - 1, j)                                    + isReproductiveCell(i + 1, j)
              + isReproductiveCell(i - 1, j + 1) + isReproductiveCell(i, j + 1) + isReproductiveCell(i + 1, j + 1);

            if (current._age + 1 >= _maxAge)
                WriteCell(nextGeneration, i, j, { 0 });
            else if (current._age > 0 && (siblings < 2 || siblings > 3))
                WriteCell(nextGeneration, i, j, { 0 });
            else if (current._age == 0 && reproductiveSiblings == 3)
                WriteCell(nextGeneration, i, j, { 1 });
            else if (current._age > 0)
                WriteCell(nextGeneration, i, j, { static_cast<unsigned long>(current._age + 1) });

            aliveCells += isAliveCell(i, j);
            reproductiveCells += isReproductiveCell(i, j);
        }
    }

    _currentGeneration._map = std::move(nextGeneration._map);

    _lastStatSnapshot._aliveCellsDelta =
        static_cast<signed long>(aliveCells - _lastStatSnapshot._aliveCells);
    _lastStatSnapshot._aliveCells = aliveCells;
    _lastStatSnapshot._reproductiveCellsDelta =
        static_cast<signed long>(reproductiveCells - _lastStatSnapshot._reproductiveCells);
    _lastStatSnapshot._reproductiveCells = reproductiveCells;
    ++_lastStatSnapshot._generation;
    if (!_lastStatSnapshot._loopDetected)
        _lastStatSnapshot._loopDetected = _cycleFinder.Apply(aliveCells);
}

size_t AdvancedFlatland::Width() const
{
    return _currentGeneration._dimensions._width;
}

size_t AdvancedFlatland::Height() const
{
    return _currentGeneration._dimensions._height;
}

const AdvancedCell& AdvancedFlatland::GetCell(size_t i, size_t j) const
{
    if (i >= _currentGeneration._dimensions._width || j >= _currentGeneration._dimensions._height)
        throw std::range_error("Bad coordinate(s) passed");

    return ReadCell(_currentGeneration, i, j);
}

unsigned long AdvancedFlatland::GetMaxAge() const
{
    return _maxAge;
}

const AdvancedFlatland::Statistics& AdvancedFlatland::GetStatistics() const
{
    return _lastStatSnapshot;
}

bool AdvancedFlatland::isAliveCell(size_t i, size_t j) const
{
    return ReadCell(_currentGeneration, i, j)._age > 0;
}

bool AdvancedFlatland::isReproductiveCell(size_t i, size_t j) const
{
    const auto age = ReadCell(_currentGeneration, i, j)._age;
    return age > 0 && age < _maxReproductivityAge;
}

}

}
