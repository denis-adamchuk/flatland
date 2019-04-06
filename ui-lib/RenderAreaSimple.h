#pragma once

#include "RenderAreaBase.h"

#include "lib/SimpleFlatland.h"

#include <QRgb>
#include <QSharedPointer>

class RenderAreaSimple : public RenderAreaBase
{
public:
    RenderAreaSimple(QWidget* parent, QSharedPointer<flatland::lib::SimpleFlatland> flatland,
                     const ScalingLimits& scalingLimits);

private:
    const flatland::lib::IFlatland& getCells() const override;
    QColor getColor(unsigned long i, unsigned long j) const override;

private:
    QSharedPointer<flatland::lib::SimpleFlatland> m_flatland;
};
