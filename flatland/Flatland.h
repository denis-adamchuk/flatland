#pragma once

class IFlatlandCreator;

// Defines a world of cells
class Flatland
{
public:
    // Creates a world of the given size
    Flatland(unsigned int width, unsigned int height, unsigned int cells, const IFlatlandCreator& creator);

    // Evaluates a next genaration of cells
    void Run();

    // Returns a flatland width
    unsigned int Width();

    // Returns a flatland height
    unsigned int Height();

    // Checks if there is a live cell at the given spot
    bool IsLive(unsigned int x, unsigned int y);

private:

};
