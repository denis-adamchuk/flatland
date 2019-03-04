#include "MapCreator.h"
#include "SimpleMapCreator.h"

#include <cstdlib>
#include <ctime>

namespace flatland
{

namespace lib
{

template <>
SimpleCellMap RandomSimpleMapCreator::Create(const RandomDistributionWithoutLimits& spec)
{
    std::srand(std::time(nullptr));

    SimpleCellMap result;
    result.resize(spec.width * spec.height);

    for (unsigned int i = 0; i < spec.height; ++i)
    {
        for (unsigned int j = 0; j < spec.width; ++j)
        {
            result[i * spec.width + j] = std::rand() % 2;
        }
    }

    return result;
}

}

}
