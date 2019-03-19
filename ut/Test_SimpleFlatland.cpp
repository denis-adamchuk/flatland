#include "lib/SimpleFlatland.h"

#include "catch2/catch.hpp"

namespace flatland
{

using namespace lib;

namespace tests
{

SimpleCellMap CreateDummyMap(const std::vector<std::vector<int>>& origin)
{
    SimpleCellMap result({ origin[0].size(), origin.size() });
    result._map.resize(origin.size() * origin[0].size());
    for (size_t j = 0; j < origin.size(); ++j)
    {
        for (size_t i = 0; i < origin[j].size(); ++i)
        {
            result._map[j * origin[j].size() + i] = !!origin[j][i];
        }
    }
    return result;
}

bool operator ==(const SimpleFlatland& lhs, const SimpleFlatland& rhs)
{
    if (lhs.Height() != rhs.Height() || lhs.Width() != rhs.Width())
        return false;

    for (size_t j = 0; j < lhs.Height(); ++j)
    {
        for (size_t i = 0; i < lhs.Width(); ++i)
        {
            if (lhs.GetCell(i, j) != rhs.GetCell(i, j))
                return false;
        }
    }

    return true;
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
    REQUIRE(flatland.GetCell(0, 1));
    REQUIRE_FALSE(flatland.GetCell(1, 0));
    REQUIRE_FALSE(flatland.GetCell(1, 1));
}

SCENARIO("Flatland throws on bad coordinates")
{
    const auto simpleMap = CreateDummyMap({{ 1 }});
    SimpleFlatland flatland(simpleMap);

    REQUIRE_THROWS_WITH(flatland.GetCell(1, 1), "Bad coordinate(s) passed");
}

SCENARIO("Flatland world next generation dimensions check")
{
    const auto simpleMap = CreateDummyMap({
        { 0, 0, 0 },
        { 1, 1, 1 },
        { 0, 0, 0 }
    });
    SimpleFlatland flatland(simpleMap);

    flatland.Run();

    REQUIRE(flatland.Width() == 3);
    REQUIRE(flatland.Height() == 3);
}

SCENARIO("Flatland world next generation -- horizontal line")
{
    const auto simpleMap = CreateDummyMap({
        { 0, 0, 0 },
        { 1, 1, 1 },
        { 0, 0, 0 }
    });
    SimpleFlatland flatland(simpleMap);

    flatland.Run();

    const auto refMap = CreateDummyMap({
        { 0, 1, 0 },
        { 0, 1, 0 },
        { 0, 1, 0 }
    });
    SimpleFlatland refFlatland(refMap);

    REQUIRE(!!(flatland == refFlatland));
}

SCENARIO("Flatland world next generation -- vertical line in a square world")
{
    const auto simpleMap = CreateDummyMap({
        { 0, 1, 0 },
        { 0, 1, 0 },
        { 0, 1, 0 }
    });
    SimpleFlatland flatland(simpleMap);

    flatland.Run();

    const auto refMap = CreateDummyMap({
        { 0, 0, 0 },
        { 1, 1, 1 },
        { 0, 0, 0 }
    });
    SimpleFlatland refFlatland(refMap);

    REQUIRE(!!(flatland == refFlatland));
}

SCENARIO("Flatland world next generation -- vertical line in a non-square world")
{
    const auto simpleMap = CreateDummyMap({
        { 1 },
        { 1 },
        { 1 }
    });
    SimpleFlatland flatland(simpleMap);

    flatland.Run();

    const auto refMap = CreateDummyMap({
        { 0 },
        { 1 },
        { 0 }
    });
    SimpleFlatland refFlatland(refMap);

    REQUIRE(!!(flatland == refFlatland));
}

SCENARIO("Flatland world next generation -- eternal box")
{
    const auto simpleMap = CreateDummyMap({
        { 1, 1 },
        { 1, 1 }
    });
    SimpleFlatland flatland(simpleMap);

    flatland.Run();

    const auto refMap = CreateDummyMap({
        { 1, 1 },
        { 1, 1 }
    });
    SimpleFlatland refFlatland(refMap);

    REQUIRE(!!(flatland == refFlatland));
}

SCENARIO("Flatland world next generation -- metamorphosis")
{
    const auto simpleMap = CreateDummyMap({
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
        { 0, 0, 1, 1, 1, 0, 0, 1, 0, 1 },
        { 0, 0, 1, 0, 0, 1, 0, 1, 1, 0 },
        { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 1, 1, 0, 0, 1, 1, 1, 0 },
        { 0, 0, 1, 0, 0, 1, 0, 0, 0, 0 }
    });
    SimpleFlatland flatland(simpleMap);

    flatland.Run();

    const auto refMap = CreateDummyMap({
       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
       { 0, 0, 0, 1, 0, 0, 0, 0, 1, 0 },
       { 0, 1, 1, 1, 1, 0, 1, 1, 0, 1 },
       { 0, 1, 1, 0, 0, 0, 1, 1, 1, 0 },
       { 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
       { 0, 1, 0, 1, 1, 0, 1, 1, 0, 0 },
       { 0, 1, 1, 1, 0, 0, 1, 1, 0, 0 }
    });
    SimpleFlatland refFlatland(refMap);

    REQUIRE(!!(flatland == refFlatland));
}
/*
SCENARIO("Flatland world next generation -- metamorphosis 2")
{
    const auto simpleMap = CreateDummyMap({
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 1, 1, 1, 0, 0, 0, 0 },
        { 0, 1, 0, 0, 0, 0, 1, 0, 0, 0 },
        { 0, 1, 0, 0, 0, 0, 1, 0, 0, 0 },
        { 0, 1, 0, 0, 0, 0, 1, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 1, 1, 1, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    });
    SimpleFlatland flatland(simpleMap);

    flatland.Run();

    const auto refMap1 = CreateDummyMap({
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 1, 0, 0, 0, 0 },
        { 0, 0, 1, 0, 1, 0, 1, 0, 0, 0 },
        { 1, 1, 1, 0, 0, 1, 1, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 1, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }
    });
    SimpleFlatland refFlatland1(refMap1);

    REQUIRE(!!(flatland == refFlatland1));

    const auto refMap2 = CreateDummyMap({
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 1, 1, 1, 0, 0, 0, 0 },
        { 0, 0, 1, 0, 1, 0, 1, 0, 0, 0 },
        { 0, 1, 1, 1, 0, 1, 1, 0, 0, 0 },
        { 0, 1, 0, 0, 1, 0, 1, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 1, 0, 0, 0, 0 },
        { 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    });
    SimpleFlatland refFlatland2(refMap2);

    flatland.Run();

    REQUIRE(!!(flatland == refFlatland2));
}
*/
}

}
