#pragma once

namespace flatland
{

namespace lib
{

struct SimpleFlatland;
struct AdvancedFlatland;

struct IFlatlandVisitor
{
    virtual void Visit(const SimpleFlatland& flatland) = 0;
    virtual void Visit(const AdvancedFlatland& flatland) = 0;
};

}

}
