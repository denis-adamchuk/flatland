#pragma once

#include "SimpleMapCreator.h"
#include "CycleFinder.h"
#include "Statistics.h"

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
    bool IsCellAlive(size_t i, size_t j) const;

    // Gather cell statistics
    const Statistics& GetStatistics() const;

private:
    bool isAliveCell(size_t i, size_t j) const;

private:
    SimpleCellMap _currentGeneration;
    Statistics _lastStatSnapshot;
    CycleFinder _cycleFinder;
};

}

}
