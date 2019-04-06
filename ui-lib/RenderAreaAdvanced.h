#pragma once

#include "RenderAreaBase.h"

#include "lib/AdvancedFlatland.h"

#include <QRgb>
#include <QSharedPointer>

class RenderAreaAdvanced : public RenderAreaBase
{
public:
    RenderAreaAdvanced(QWidget* parent, QSharedPointer<flatland::lib::AdvancedFlatland> flatland,
                       const ScalingLimits& scalingLimits);

private:
    const flatland::lib::IFlatland& getCells() const override;
    QColor getColor(unsigned long i, unsigned long j) const override;

private:
    QSharedPointer<flatland::lib::AdvancedFlatland> m_flatland;
};
