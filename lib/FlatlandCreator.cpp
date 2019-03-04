#include "FlatlandCreator.h"

template <>
SimpleCellMap SimpleMapCreator::Create(const SimpleSpec& spec)
{
    return {};
}
