#pragma once

#include "FlatlandItf.h"
#include "AdvancedMapCreator.h"
#include "Statistics.h"

namespace flatland
{

namespace lib
{

// Defines a world of cells which have ages
class AdvancedFlatland : public IFlatland
{
public:
    // Creates a world of the given size
    AdvancedFlatland(const AdvancedCellMap& flatlandMap, unsigned long maxAge,
        unsigned long maxReproductivityAge);

    // Performs a single step of cell evolution within the whole world
    bool Run() override;

    // Returns a flatland width
    size_t Width() const override;

    // Returns a flatland height
    size_t Height() const override;

    // Checks if there is a live cell at the given spot (i - col, j - row)
    bool IsCellAlive(size_t i, size_t j) const override;

    // Returns age of c cell at the given spot (i - col, j - row)
    unsigned long GetCellAge(size_t i, size_t j) const;

    // Returns a maximum age specified in Flatland constructor
    unsigned long GetMaxAge() const;

    // Gather cell statistics
    const StatisticsMap& GetStatistics() const override;

private:
    // Checks if a cell with passed age can born new cells
    bool isReproductiveAge(unsigned long age) const;

private:
    AdvancedCellMap _currentGeneration;
    StatisticsMap _lastStatSnapshot;
    unsigned long _maxAge;
    unsigned long _maxReproductivityAge;
};

inline bool AdvancedFlatland::isReproductiveAge(unsigned long age) const
{
    return age > 0 && age < _maxReproductivityAge;
}

}

}
