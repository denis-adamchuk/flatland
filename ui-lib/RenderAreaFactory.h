#pragma once

#include "RenderAreaBase.h"

#include "lib/SimpleFlatland.h"
#include "RenderAreaSimple.h"

#include "lib/AdvancedFlatland.h"
#include "RenderAreaAdvanced.h"

#include <QWidget>

inline RenderAreaBase* CreateRenderArea(QWidget* parent, QSharedPointer<flatland::lib::SimpleFlatland> flatland,
                                        const ScalingLimits& scalingLimits)
{
    return new RenderAreaSimple(parent, flatland, scalingLimits);
}

inline RenderAreaBase* CreateRenderArea(QWidget* parent, QSharedPointer<flatland::lib::AdvancedFlatland> flatland,
                                        const ScalingLimits& scalingLimits)
{
    return new RenderAreaAdvanced(parent, flatland, scalingLimits);
}

