#pragma once

#include "FlatlandItf.h"
#include "AdvancedMapCreator.h"
#include "Statistics.h"

namespace flatland
{

namespace lib
{

// Defines a world of cells
class AdvancedFlatland : public IFlatland
{
public:
    // Creates a world of the given size
    AdvancedFlatland(const AdvancedCellMap& flatlandMap, unsigned long maxAge,
        unsigned long maxReproductivityAge);

    // Evaluates a next generation of cells
    bool Run() override;

    // Returns a flatland width
    size_t Width() const override;

    // Returns a flatland height
    size_t Height() const override;

    // Checks if there is a live cell at the given spot (i - col, j - row)
    bool IsCellAlive(size_t i, size_t j) const override;

    // Returns age of c cell at the given spot (i - col, j - row)
    unsigned long GetCellAge(size_t i, size_t j) const;

    // Returns a maximum age saved on flatland construction
    unsigned long GetMaxAge() const;

    // Gather cell statistics
    const StatisticsMap& GetStatistics() const override;

private:
    bool isReproductiveAge(unsigned long age) const;

private:

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
