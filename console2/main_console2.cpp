#include "lib/SimpleFlatland.h"
#include "lib/AdvancedFlatland.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <queue>

using namespace flatland;
using namespace flatland::lib;

template <typename TFlatland>
void run(TFlatland&& flatland)
{
    while (flatland.Run())
    {
        for (const auto& i: flatland.GetStatistics())
        {
            const auto& name = i.first;
            const auto& val = i.second;
            std::cout << name << ": " << val.GetAsText() << "     ";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cout << "Use: console2 <Width> <Height> [MaxAge] [MaxReproductivityAge]";
        return -1;
    }

    const unsigned long width = static_cast<unsigned long>(atol(argv[1]));
    const unsigned long height = static_cast<unsigned long>(atol(argv[2]));
    if (width == 0 || height == 0)
    {
        std::cout << "Invalid argument";
        return -1;
    }

    const unsigned long maxAge = argc < 5 ? 0 : static_cast<unsigned long>(atol(argv[3]));
    const unsigned long maxReproductivityAge = argc < 5 ? 0 : static_cast<unsigned long>(atol(argv[4]));

    if (maxAge == 0)
    {
        run(SimpleFlatland(CreateSimpleMap(RandomDistributionWithoutLimits{width, height})));
    }
    else if (maxReproductivityAge != 0)
    {
        run(AdvancedFlatland(CreateAdvancedMap(RandomDistributionWithoutLimits{width, height}),
            maxAge, maxReproductivityAge));
    }
    else
    {
        std::cout << "Invalid argument";
        return -1;
    }

    return 0;
}
