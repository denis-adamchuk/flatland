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

    RandomSimpleMapCreator creator;
    const auto simpleMap = creator.Create(spec);

    REQUIRE(simpleMap.size() == spec.width * spec.height);
}

//SCENARIO("FlatlandCreator tests", "Created world content check")
//{
//    const SimpleSpec spec = { 17, 18, 25 };

//    SimpleMapCreator creator;
//    const auto simpleMap = creator.Create(spec);

//    // TODO or invert loop?
//    unsigned int liveCells = 0;
//    for (unsigned int i = 0; i < height; ++i)
//    {
//        for (unsigned int j = 0; j < width; ++j)
//        {
//            if (simpleMap[i * width + j])
//            {
//                ++liveCells;
//            }
//        }
//    }
//    REQUIRE(liveCells == cells);
//}

}

}
