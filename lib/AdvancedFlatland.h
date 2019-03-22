#pragma once

#include "AdvancedMapCreator.h"
#include "Statistics.h"

namespace flatland
{

namespace lib
{

// Defines a world of cells
class AdvancedFlatland
{
public:
    // Creates a world of the given size
    AdvancedFlatland(const AdvancedCellMap& flatlandMap, unsigned long maxAge,
        unsigned long maxReproductivityAge);

    // Evaluates a next generation of cells
    void Run();

    // Returns a flatland width
    size_t Width() const;

    // Returns a flatland height
    size_t Height() const;

    // Checks if there is a live cell at the given spot (i - col, j - row)
    bool IsCellAlive(size_t i, size_t j) const;

    // Returns age of c cell at the given spot (i - col, j - row)
    unsigned long GetCellAge(size_t i, size_t j) const;

    // Returns a maximum age saved on flatland construction
    unsigned long GetMaxAge() const;

    // Gather cell statistics
    const AdvancedStatistics& GetStatistics() const;

private:
    bool isAliveCell(size_t i, size_t j) const;
    bool isReproductiveCell(size_t i, size_t j) const;

private:
    AdvancedCellMap _currentGeneration;
    AdvancedStatistics _lastStatSnapshot;
    unsigned long _maxAge;
    unsigned long _maxReproductivityAge;
};

}

}
