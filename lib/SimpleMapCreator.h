#pragma once

#include <vector>

namespace flatland
{

namespace lib
{

struct MapDimensions
{
    size_t _width;
    size_t _height;
};

// Map is coded with bools
using SimpleCell = bool;
struct SimpleCellMap
{
    const MapDimensions _dimensions;
    std::vector<SimpleCell> _map;

    SimpleCellMap(const MapDimensions& dimensions)
        : _dimensions(dimensions)
    {
        // TODO Test if it really optimizes anything?
        _map.reserve(_dimensions._height * _dimensions._width);
    }
};

// No limits on amount or percentage of alive cells on a original map
struct RandomDistributionWithoutLimits
{
    MapDimensions _dimensions;
};
SimpleCellMap CreateSimpleMap(const RandomDistributionWithoutLimits& spec);


// Helpers
inline void WriteCell(SimpleCellMap& map, size_t i, size_t j, bool value)
{
    map._map[j * map._dimensions._width + i] = value;
}

inline bool ReadCell(const SimpleCellMap& map, size_t i, size_t j)
{
    if (i >= map._dimensions._width || j >= map._dimensions._height)
        return false; // a border of the world

    return map._map[j * map._dimensions._width + i];
}

}

}
