#include "RenderAreaSimple.h"

RenderAreaSimple::RenderAreaSimple(QWidget* parent, QSharedPointer<flatland::lib::SimpleFlatland> flatland,
                                   const ScalingLimits& scalingLimits)
    : RenderAreaBase(parent, scalingLimits)
    , m_flatland(flatland)
{
}

const flatland::lib::IFlatland &RenderAreaSimple::getCells() const
{
    return *m_flatland;
}

QColor RenderAreaSimple::getColor(unsigned long, unsigned long) const
{
    return Qt::green;
}
