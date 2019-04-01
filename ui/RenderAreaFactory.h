#pragma once

#include "RenderAreaBase.h"

#include "lib/SimpleFlatland.h"
#include "RenderAreaSimple.h"

#include "lib/AdvancedFlatland.h"
#include "RenderAreaAdvanced.h"

#include <QWidget>

inline RenderAreaBase* CreateRenderArea(QWidget* parent, QSharedPointer<flatland::lib::SimpleFlatland> flatland)
{
    return new RenderAreaSimple(parent, flatland);
}

inline RenderAreaBase* CreateRenderArea(QWidget* parent, QSharedPointer<flatland::lib::AdvancedFlatland> flatland)
{
    return new RenderAreaAdvanced(parent, flatland);
}

