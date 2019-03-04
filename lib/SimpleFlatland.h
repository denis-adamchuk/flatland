#pragma once

#include "SimpleMapCreator.h"

namespace flatland
{

namespace lib
{

// Defines a world of cells
class SimpleFlatland
{
public:
    // Creates a world of the given size
    SimpleFlatland(const SimpleCellMap& flatlandMap);

    // Evaluates a next generation of cells
    void Run();

    // Returns a flatland width
    unsigned int Width();

    // Returns a flatland height
    unsigned int Height();

    // Checks if there is a live cell at the given spot
    bool GetCell(unsigned int x, unsigned int y);

private:
    SimpleCellMap m_current;
};

}

}
