#include "lib/SimpleFlatland.h"
#include "lib/AdvancedFlatland.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <queue>

using namespace flatland;
using namespace flatland::lib;

void printStatistics(const SimpleFlatland& flatland)
{
    const auto& stat = flatland.GetStatistics();
    std::cout
        << "Generation: " << stat._generation
        << " Alive cells: " << stat._aliveCells
        << (stat._loopDetected ? " [LOOP]" : "")
        << "\n";
    std::cout.flush();
}

void printStatistics(const AdvancedFlatland& flatland)
{
    const auto& stat = flatland.GetStatistics();
    std::cout
        << "Generation: " << stat._generation
        << " Alive cells: " << stat._aliveCells
        << " Reproductive cells: " << stat._reproductiveCells
        << "\n";
    std::cout.flush();
}

template <typename TFlatland>
void run(TFlatland&& flatland)
{
    const auto& stat = flatland.GetStatistics();
    while (stat._aliveCells > 0)
    {
        printStatistics(flatland);
        flatland.Run();
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
