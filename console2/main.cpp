#include "lib/AdvancedFlatland.h"

#include <iostream>
#include <chrono>
#include <thread>

using namespace flatland;
using namespace flatland::lib;

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        std::cout << "Use: console2 <Width> <Height> <MaxAge> <MaxReproductivityAge>";
        return -1;
    }

    const unsigned long width = static_cast<unsigned long>(atol(argv[1]));
    const unsigned long height = static_cast<unsigned long>(atol(argv[2]));
    const unsigned long maxAge = static_cast<unsigned long>(atol(argv[3]));
    const unsigned long maxReproductivityAge = static_cast<unsigned long>(atol(argv[4]));
    if (width == 0 || height == 0 || maxAge == 0 || maxReproductivityAge == 0)
    {
        std::cout << "Invalid argument";
        return -1;
    }

    AdvancedFlatland flatland(CreateAdvancedMap(RandomDistributionWithoutLimits{width, height}),
        maxAge, maxReproductivityAge);

    while (true)
    {
        const auto& stat = flatland.GetStatistics();

        std::cout
            << "\rGeneration: " << stat._generation
            << " Alive cells: " << stat._aliveCells
            << " Reproductive cells: " << stat._reproductiveCells
            << std::endl;

        if (stat._aliveCells <= 0)
        {
            break;
        }

        flatland.Run();

        if (stat._aliveCells <= 0)
        {
            break;
        }

        static const unsigned long sc_delay = 20; // ms
        std::this_thread::sleep_for(std::chrono::milliseconds(sc_delay));
    }

    return 0;
}
