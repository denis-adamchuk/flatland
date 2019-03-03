#pragma once

#include "FlatlandCreatorItf.h"
#include "Flatland.h"

// Defines a creator of worlds
class FlatlandCreator : public IFlatlandCreator
{
public:
    // Create a world
    virtual Flatland CreateFlatland(unsigned int width, unsigned int height, unsigned int cells);
};
