#pragma once

#include <vector>

struct MapDimensions
{
    size_t _width{ 0 };
    size_t _height{ 0 };
};

// No limits on amount or percentage of alive cells on a original map
struct RandomDistributionWithoutLimits
{
    MapDimensions _dimensions;
};

template <typename TMapElement>
struct FlatlandMap
{
    typedef TMapElement value_type;
    typedef typename std::vector<TMapElement>::const_reference const_reference;

    const MapDimensions _dimensions;
    std::vector<TMapElement> _map;

    FlatlandMap(const MapDimensions& dimensions)
        : _dimensions(dimensions)
    {
    }
};

template <typename TMap>
void WriteCell(TMap& map, size_t i, size_t j, const typename TMap::value_type& value)
{
    map._map[j * map._dimensions._width + i] = value;
}

template <typename TMap>
const typename TMap::const_reference ReadCell(const TMap& map, size_t i, size_t j)
{
    static const typename TMap::value_type sc_dummy{};
    if (i >= map._dimensions._width || j >= map._dimensions._height)
        return sc_dummy; // a border of the world

    return map._map[j * map._dimensions._width + i];
}
