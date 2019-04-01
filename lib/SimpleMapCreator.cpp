#include "SimpleMapCreator.h"

#include <cstdlib>
#include <ctime>

namespace flatland
{

namespace lib
{

SimpleCellMap CreateSimpleMap(const RandomDistributionWithoutLimits& spec)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    SimpleCellMap result(spec._dimensions);
    result._map.resize(spec._dimensions._width * spec._dimensions._height);

    for (size_t j = 0; j < spec._dimensions._height; ++j)
    {
        for (size_t i = 0; i < spec._dimensions._width; ++i)
        {
            WriteCell(result, i, j, std::rand() % 2);
        }
    }

    return result;
}

}

}
