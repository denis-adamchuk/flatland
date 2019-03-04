#pragma once

#include <vector>

// Defines a creator of worlds
template <typename TSpec, typename TCell, typename TCellMap>
class MapCreator
{
    // Create a world
  TCellMap Create(const TSpec& spec);
};
