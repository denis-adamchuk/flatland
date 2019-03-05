#include "../lib/SimpleMapCreator.h"

#include "../catch2/catch.hpp"

namespace flatland
{

using namespace lib;

namespace tests
{

SCENARIO("SimpleMapCreator tests", "Created world dimensions check")
{
    const RandomDistributionWithoutLimits spec = { 17, 18 };

    const auto simpleMap = CreateSimpleMap(spec);

    REQUIRE(simpleMap._dimensions._height == spec._dimensions._height);
    REQUIRE(simpleMap._dimensions._width == spec._dimensions._width);
    REQUIRE(simpleMap._map.size() == spec._dimensions._width * spec._dimensions._height);
}

}

}
