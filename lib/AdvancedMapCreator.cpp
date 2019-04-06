#include "AdvancedMapCreator.h"

#include <optional>
#include <cstdlib>
#include <ctime>
#include <cmath>

namespace flatland
{

namespace lib
{

AdvancedCellMap CreateAdvancedMap(const RandomDistributionWithoutLimits& spec)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    AdvancedCellMap result(spec._dimensions);
    result._map.resize(spec._dimensions._width * spec._dimensions._height);

    for (size_t j = 0; j < spec._dimensions._height; ++j)
    {
        for (size_t i = 0; i < spec._dimensions._width; ++i)
        {
            if (std::rand() % 2)
            {
                const AdvancedCell cell = { 1 }; // all cells have the same age in the beginning (just hypothesis)
                WriteCell(result, i, j, cell);
            }
        }
    }

    return result;
}

}

}
