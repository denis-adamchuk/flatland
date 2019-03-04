#pragma once

#include <vector>

namespace flatland
{

namespace lib
{

// Defines a creator of worlds
template <typename TSpec, typename TCell, typename TCellMap>
class MapCreator
{
public:
    // Create a world
    TCellMap Create(const TSpec& spec);
};

}

}
