#include "AdvancedFlatland.h"
#include "Common.h"

#include <stdexcept>

namespace flatland
{

namespace lib
{

AdvancedFlatland::AdvancedFlatland(const AdvancedCellMap& flatlandMap, unsigned long maxAge,
    unsigned long maxReproductivityAge)
    : _currentGeneration(flatlandMap)
    , _maxAge(maxAge)
    , _maxReproductivityAge(maxReproductivityAge)
{
    unsigned long aliveCells = 0;
    unsigned long reproductiveCells = 0;

    for (size_t j = 0; j < _currentGeneration._dimensions._height; ++j)
    {
        for (size_t i = 0; i < _currentGeneration._dimensions._width; ++i)
        {
            if (isAliveCell(i, j))
                ++aliveCells;
            if (isReproductiveCell(i, j))
                ++reproductiveCells;
        }
    }

    _lastStatSnapshot["AliveCells"].Set(aliveCells);
    _lastStatSnapshot["ReproductiveCells"].Set(reproductiveCells);
    _lastStatSnapshot["AliveCellsDelta"].Set(static_cast<long>(aliveCells));
    _lastStatSnapshot["ReproductiveCellsDelta"].Set(static_cast<long>(reproductiveCells));
    _lastStatSnapshot["Generation"].Set(static_cast<unsigned long>(0));
}

bool AdvancedFlatland::Run()
{
    unsigned long aliveCells = 0;
    unsigned long reproductiveCells = 0;

    AdvancedCellMap nextGeneration(_currentGeneration);
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
            const auto reproductiveSiblings = CountSiblings(i, j,
                [this](size_t i, size_t j)
            {
                return isReproductiveCell(i, j);
            });

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

    auto prevAliveCells = _lastStatSnapshot["AliveCells"].Get<unsigned long>();
    _lastStatSnapshot["AliveCellsDelta"].Set(static_cast<long>(aliveCells - prevAliveCells));
    _lastStatSnapshot["AliveCells"].Set(aliveCells);
    auto prevReproductiveCells = _lastStatSnapshot["ReproductiveCells"].Get<unsigned long>();
    _lastStatSnapshot["ReproductiveCellsDelta"].Set(static_cast<long>(reproductiveCells - prevReproductiveCells));
    _lastStatSnapshot["ReproductiveCells"].Set(reproductiveCells);
    auto prevGeneration = _lastStatSnapshot["Generation"].Get<unsigned long>();
    _lastStatSnapshot["Generation"].Set(prevGeneration + 1);

    return aliveCells > 0;
}

size_t AdvancedFlatland::Width() const
{
    return _currentGeneration._dimensions._width;
}

size_t AdvancedFlatland::Height() const
{
    return _currentGeneration._dimensions._height;
}

bool AdvancedFlatland::IsCellAlive(size_t i, size_t j) const
{
    return GetCellAge(i, j) > 0;
}

unsigned long AdvancedFlatland::GetCellAge(size_t i, size_t j) const
{
    if (i >= _currentGeneration._dimensions._width || j >= _currentGeneration._dimensions._height)
        throw std::range_error("Bad coordinate(s) passed");

    return ReadCell(_currentGeneration, i, j)._age;
}

unsigned long AdvancedFlatland::GetMaxAge() const
{
    return _maxAge;
}

const StatisticsMap& AdvancedFlatland::GetStatistics() const
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
