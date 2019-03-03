#include "../catch2/catch.hpp"
#include "../flatland/Flatland.h"
#include "../flatland/FlatlandCreator.h"
#include "mocks/FlatlandCreatorMock.h"

SCENARIO("FlatlandCreator tests", "Created world dimensions check")
{
    const unsigned int width = 17;
    const unsigned int height = 18;
    const unsigned int cells = 25;

    FlatlandCreator creator;
    const auto flatland = creator.CreateFlatland(width, height, cells);

    REQUIRE(flatland.Width() == expectedWidth);
    REQUIRE(flatland.Height() == expectedHeight);
}

SCENARIO("FlatlandCreator tests", "Created world content check")
{
    const unsigned int width = 17;
    const unsigned int height = 18;
    const unsigned int cells = 25;

    FlatlandCreator creator;
    auto flatland = creator.CreateFlatland(width, height, cells);

    unsigned int liveCells = 0;
    for (unsigned int x = 0; x < width; ++x)
    {
        for (unsigned int y = 0; y < height; ++y)
        {
            if (flatland.IsLive(x, y))
            {
                ++liveCells;
            }
        }
    }
    REQUIRE(liveCells == cells);
}

SCENARIO("Flatland tests", "World dimensions check")
{
    unsigned int expectedWidth = 17;
    unsigned int expectedHeight = 18;

    Flatland flatland(expectedWidth, expectedHeight, 0, FlatlandCreatorMock());

    REQUIRE(flatland.Width() == expectedWidth);
    REQUIRE(flatland.Height() == expectedHeight);
}

SCENARIO("Flatland tests", "Next generation check")
{
    const unsigned int width = 17;
    const unsigned int height = 18;
    const unsigned int cells = 25;

    FlatlandCreatorMock creator;
    Flatland flatland(width, height, cells, creator);


    FlatlandCreator creator;
    auto flatland = creator.CreateFlatland(width, height, cells);

    unsigned int liveCells = 0;
    for (unsigned int x = 0; x < width; ++x)
    {
        for (unsigned int y = 0; y < height; ++y)
        {
            if (flatland.IsLive(x, y))
            {
                ++liveCells;
            }
        }
    }
    REQUIRE(liveCells == cells);
}
