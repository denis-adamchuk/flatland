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
    size_t Width() const;

    // Returns a flatland height
    size_t Height() const;

    // Checks if there is a live cell at the given spot (i - col, j - row)
    bool GetCell(size_t i, size_t j) const;

private:
    SimpleCellMap _current;
};

}

}
