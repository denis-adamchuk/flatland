#include "RenderAreaAdvanced.h"

#include <QColor>

RenderAreaAdvanced::RenderAreaAdvanced(QWidget* parent, QSharedPointer<flatland::lib::AdvancedFlatland> flatland)
    : RenderAreaBase(parent)
    , m_flatland(flatland)
{

}

const flatland::lib::IFlatland &RenderAreaAdvanced::getCells() const
{
    return *m_flatland;
}

QColor RenderAreaAdvanced::getColor(unsigned long i, unsigned long j) const
{
    const auto maxAge = m_flatland->GetMaxAge();
    const auto age = m_flatland->GetCellAge(i, j);

    int h = 0;
    int s = 0;
    int v = 0;

         if (age > maxAge * 0.0 && age < maxAge * 0.2)
        h = 120; // G
    else if (age > maxAge * 0.2 && age < maxAge * 0.4)
        h = 30;  // Orange
    else if (age > maxAge * 0.4 && age < maxAge * 0.6)
        h = 0;   // R
    else if (age > maxAge * 0.6 && age < maxAge * 0.8)
        h = 300; // Magenta
    else if (age > maxAge * 0.8 && age < maxAge * 1.0)
        h = 240; // B
    const auto ageTo255 = static_cast<int>(255. * age / maxAge);
    static const auto lowestV = 130;
    s = v = std::max(lowestV, 255 - ageTo255);

    return QColor::fromHsv(h, s, v);
}

