#include "RenderAreaSimple.h"

RenderAreaSimple::RenderAreaSimple(QWidget* parent, QSharedPointer<flatland::lib::SimpleFlatland> flatland)
    : RenderAreaBase(parent)
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

