#pragma once

#include "Flatland.h"

// Defines a creator of worlds
class IFlatlandCreator
{
public:
    // Create a world
    virtual Flatland CreateFlatland(unsigned int width, unsigned int height, unsigned int cells) = 0;
};
