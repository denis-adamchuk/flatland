#include "SimpleFlatland.h"

#include <stdexcept>

namespace flatland
{

namespace lib
{

SimpleFlatland::SimpleFlatland(const SimpleCellMap& flatlandMap)
    : _current(flatlandMap)
{
}

void SimpleFlatland::Run()
{
    SimpleCellMap nextGeneration(_current);
    for (size_t j = 0; j < _current._dimensions._height; ++j)
    {
        for (size_t i = 0; i < _current._dimensions._width; ++i)
        {
            const auto current = ReadCell(_current, i, j);
            const auto siblings =
                ReadCell(_current, i - 1, j - 1) + ReadCell(_current, i    , j - 1) + ReadCell(_current, i + 1, j - 1)
              + ReadCell(_current, i - 1, j    )                                    + ReadCell(_current, i + 1, j    )
              + ReadCell(_current, i - 1, j + 1) + ReadCell(_current, i    , j + 1) + ReadCell(_current, i + 1, j + 1);

            if (current && (siblings < 2 || siblings > 3))
                WriteCell(nextGeneration, i, j, false);
            else if (!current && siblings == 3)
                WriteCell(nextGeneration, i, j, true);
        }
    }
    std::swap(nextGeneration._map, _current._map);
}

size_t SimpleFlatland::Width() const
{
    return _current._dimensions._width;
}

size_t SimpleFlatland::Height() const
{
    return _current._dimensions._height;
}

bool SimpleFlatland::GetCell(size_t i, size_t j) const
{
    if (i >= _current._dimensions._width || j >= _current._dimensions._height)
        throw std::range_error("Bad coordinate(s) passed");

    return ReadCell(_current, i, j);
}

}

}
