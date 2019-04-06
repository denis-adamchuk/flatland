#pragma once

#include "Common.h"

#include <vector>

namespace flatland
{

namespace lib
{

// 'Advanced' cell has age (comparing to a 'Simple' cell which has nothing at all)
struct AdvancedCell
{
    unsigned long _age{ 0 };
};

// Creates a random map, see RandomDistributionWithoutLimits
using AdvancedCellMap = FlatlandMap<AdvancedCell>;
AdvancedCellMap CreateAdvancedMap(const RandomDistributionWithoutLimits& spec);


}

}
