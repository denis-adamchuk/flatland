#pragma once

#include "Common.h"

#include <vector>

namespace flatland
{

namespace lib
{

struct AdvancedCell
{
    unsigned long _age{ 0 };
};

using AdvancedCellMap = FlatlandMap<AdvancedCell>;
AdvancedCellMap CreateAdvancedMap(const RandomDistributionWithoutLimits& spec);


}

}
