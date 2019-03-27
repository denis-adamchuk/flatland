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
    static const typename TMap::value_type defaultItem{ false };
    static std::vector<typename TMap::value_type> items;
    if (items.empty())
        items.emplace_back(defaultItem);
    if (i >= map._dimensions._width || j >= map._dimensions._height)
        return items[0];

    return map._map[j * map._dimensions._width + i];
}

template <typename TFnCheck>
unsigned long CountSiblings(size_t i, size_t j, const TFnCheck& fn)
{
    return
        fn(i - 1, j - 1) + fn(i, j - 1) + fn(i + 1, j - 1)
      + fn(i - 1, j)                    + fn(i + 1, j)
      + fn(i - 1, j + 1) + fn(i, j + 1) + fn(i + 1, j + 1);
}

}

}
