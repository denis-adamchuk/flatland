#pragma once

#include "MapCreator.h"

#include <vector>

namespace flatland
{

namespace lib
{

// Map is coded with bools
using SimpleCell = bool;
using SimpleCellMap = std::vector<SimpleCell>;
template <typename TDistribution>
using SimpleMapCreator = MapCreator<TDistribution, SimpleCell, SimpleCellMap>;

// No limits on amount or percentage of alive cells on a original map
struct RandomDistributionWithoutLimits
{
    unsigned int width;
    unsigned int height;
};
using RandomSimpleMapCreator = SimpleMapCreator<RandomDistributionWithoutLimits>;

}

}
