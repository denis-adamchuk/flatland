#pragma once

#include "FlatlandItf.h"
#include "SimpleMapCreator.h"
#include "CycleFinder.h"
#include "Statistics.h"

namespace flatland
{

namespace lib
{

// Defines a world of cells
class SimpleFlatland : public IFlatland
{
public:
    // Creates a world of the given size
    SimpleFlatland(const SimpleCellMap& flatlandMap);

    // Evaluates a next generation of cells
    bool Run() override;

    // Returns a flatland width
    size_t Width() const override;

    // Returns a flatland height
    size_t Height() const override;

    // Checks if there is a live cell at the given spot (i - col, j - row)
    bool IsCellAlive(size_t i, size_t j) const override;

    // Gather cell statistics
    const StatisticsMap& GetStatistics() const override;

private:
    SimpleCellMap _currentGeneration;
    StatisticsMap _lastStatSnapshot;
    CycleFinder _cycleFinder;
};

}

}
