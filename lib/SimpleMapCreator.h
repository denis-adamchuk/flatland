#pragma once

#include "Common.h"

#include <vector>

namespace flatland
{

namespace lib
{

using SimpleCell = bool;
using SimpleCellMap = FlatlandMap<SimpleCell>;
SimpleCellMap CreateSimpleMap(const RandomDistributionWithoutLimits& spec);

}

}
