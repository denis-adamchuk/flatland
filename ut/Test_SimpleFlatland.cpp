#include "../lib/SimpleFlatland.h"

#include "../catch2/catch.hpp"

namespace flatland
{

using namespace lib;

namespace tests
{

SimpleCellMap CreateDummyMap(const std::vector<std::vector<int>>& origin)
{
    SimpleCellMap result;
    result.resize(origin.size() * origin[0].size());
    for (size_t i = 0; i < origin.size(); ++i)
    {
        for (size_t j = 0; j < origin[i].size(); ++j)
        {
            result[i * origin[i].size() + j] = !!origin[i][j];
        }
    }
    return result;
}

SCENARIO("Flatland world original dimensions check")
{
    const auto simpleMap = CreateDummyMap({
        { 1, 0 },
        { 1, 0 }
    });
    SimpleFlatland flatland(simpleMap);

    REQUIRE(flatland.Width() == 2);
    REQUIRE(flatland.Height() == 2);
    REQUIRE(flatland.GetCell(0, 0));
    REQUIRE(flatland.GetCell(1, 0));
    REQUIRE_FALSE(flatland.GetCell(0, 1));
    REQUIRE_FALSE(flatland.GetCell(1, 1));
}

SCENARIO("Flatland world next generation dimensions check")
{
    const auto simpleMap = CreateDummyMap({
        { 1, 0 },
        { 1, 0 }
    });
    SimpleFlatland flatland(simpleMap);

    flatland.Run();

    REQUIRE(flatland.Width() == 2);
    REQUIRE(flatland.Height() == 2);
}

SCENARIO("Flatland world next generation content check")
{
    const auto simpleMap = CreateDummyMap({
        { 1, 0 },
        { 1, 0 }
    });
    SimpleFlatland flatland(simpleMap);

    flatland.Run();

    REQUIRE(flatland.GetCell(0, 0));
    REQUIRE(flatland.GetCell(1, 0));
    REQUIRE_FALSE(flatland.GetCell(0, 1));
    REQUIRE_FALSE(flatland.GetCell(1, 1));
}

}

}
