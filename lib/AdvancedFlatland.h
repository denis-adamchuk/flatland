#pragma once

#include "AdvancedMapCreator.h"

namespace flatland
{

namespace lib
{

// Defines a world of cells
class AdvancedFlatland
{
public:
    struct Statistics
    {
        unsigned long _aliveCells { 0 };
          signed long _aliveCellsDelta { 0 };
        unsigned long _reproductiveCells { 0 };
          signed long _reproductiveCellsDelta { 0 };
        unsigned long _generation { 0 };
    };

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
    const AdvancedCell& GetCell(size_t i, size_t j) const;

    unsigned long GetMaxAge() const;

    const Statistics& GetStatistics() const;

private:
    bool isAliveCell(size_t i, size_t j) const;
    bool isReproductiveCell(size_t i, size_t j) const;

private:
    AdvancedCellMap _currentGeneration;
    Statistics _lastStatSnapshot;
    unsigned long _maxAge;
    unsigned long _maxReproductivityAge;
};

}

}
