#pragma once

#include "ui-lib/AdjustableTimer.h"
#include "ui-lib/RenderAreaBase.h"

#include "lib/SimpleFlatland.h"
#include "lib/AdvancedFlatland.h"

#include <QWidget>
#include <QSharedPointer>

#include <optional>

class QTimer;
class QTouchEvent;

using namespace flatland::lib;

// Main window for Android UI version of Flatland application
class Window : public QWidget
{
public:
    Window(QSharedPointer<SimpleFlatland> simpleFlatland,
           QSharedPointer<AdvancedFlatland> advancedFlatland,
           unsigned long defaultScale);

protected:
    QSize sizeHint() const override;
    bool event(QEvent* event) override;

private:
    void processOneTouchPoint(const QTouchEvent& event);
    void processTwoTouchPoints(const QTouchEvent& event);
    void toggleRenderArea();
    void changeSpeed(int delta);

    void updateScale(qreal scale, QPoint pt);
    void updateTopLeft(QPoint currentPoint, QPoint prevPoint);

private:
    AdjustableTimer m_timer;
    RenderAreaBase* m_simpleRenderArea = nullptr;
    RenderAreaBase* m_advancedRenderArea = nullptr;
    bool m_isActiveSimpleFlatland = false;

    std::optional<qreal> m_initialLength;
    std::optional<unsigned long> m_initialScale;
};
