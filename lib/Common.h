#pragma once

#include <vector>
#include <functional>
#include <initializer_list>

namespace flatland
{

namespace lib
{

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
    typedef typename std::vector<TMapElement>::const_reference const_reference;
    typedef typename std::vector<TMapElement>::value_type value_type;

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
    return map._map[j * map._dimensions._width + i];
}

}

}
