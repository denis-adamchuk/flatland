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
            const auto currentCellAge = GetCellAge(i, j);
            aliveCells += (currentCellAge > 0);
            reproductiveCells += isReproductiveAge(currentCellAge);
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
            size_t siblings = 0;
            size_t reproductiveSiblings = 0;
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

                    const auto cellAge = GetCellAge(static_cast<size_t>(x) + i, static_cast<size_t>(y) + j);
                    siblings += (cellAge > 0);
                    reproductiveSiblings += isReproductiveAge(cellAge);
                }
            }

            const auto currentCellAge = GetCellAge(i, j);
            if (currentCellAge + 1 >= _maxAge)
                WriteCell(nextGeneration, i, j, { 0 });
            else if (currentCellAge > 0 && (siblings < 2 || siblings > 3))
                WriteCell(nextGeneration, i, j, { 0 });
            else if (currentCellAge == 0 && reproductiveSiblings == 3)
                WriteCell(nextGeneration, i, j, { 1 });
            else if (currentCellAge > 0)
                WriteCell(nextGeneration, i, j, { static_cast<unsigned long>(currentCellAge + 1) });

            aliveCells += (currentCellAge > 0);
            reproductiveCells += isReproductiveAge(currentCellAge);
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

}

}
