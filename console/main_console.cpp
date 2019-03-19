#include "lib/SimpleFlatland.h"

#include <iostream>
#include <chrono>
#include <thread>

using namespace flatland;
using namespace flatland::lib;

int main(int, char*[])
{
    size_t width;
    std::cout << "World width: ";
    std::cin >> width;

    size_t height;
    std::cout << "World height: ";
    std::cin >> height;

    size_t delay;
    std::cout << "Print delay (ms): ";
    std::cin >> delay;

    size_t iGeneration = 0;
    SimpleFlatland flatland(CreateSimpleMap(RandomDistributionWithoutLimits{width, height}));
    while (true)
    {
        std::cout << "Generation #" << iGeneration << ":" << std::endl;
        for (size_t j = 0; j < flatland.Height(); ++j)
        {
            for (size_t i = 0; i < flatland.Width(); ++i)
            {
                if (i != 0)
                    std::cout << "   ";

                const auto cell = flatland.GetCell(i, j);
                std::cout << (cell ? "." : " ");
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        std::cout << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(delay));

        flatland.Run();
        ++iGeneration;
    }

    return 0;
}
