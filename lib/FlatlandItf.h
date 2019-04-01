#pragma once

#include "Statistics.h"

namespace flatland
{

namespace lib
{

struct IFlatland
{
    // Evaluates a next generation of cells
    virtual bool Run() = 0;

    // Returns a cell collection width
    virtual size_t Width() const = 0;

    // Returns a cell collection height
    virtual size_t Height() const = 0;

    // Checks if there is a live cell at the given spot (i - col, j - row)
    virtual bool IsCellAlive(size_t i, size_t j) const = 0;

    // Returns flatland statistics
    virtual const StatisticsMap& GetStatistics() const = 0;

    virtual ~IFlatland() {}
};

}

}
