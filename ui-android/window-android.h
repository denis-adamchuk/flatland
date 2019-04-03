#pragma once

#include "ui/AdjustableTimer.h"
#include "ui/RenderAreaBase.h"

#include "lib/SimpleFlatland.h"
#include "lib/AdvancedFlatland.h"

#include <QWidget>
#include <QSharedPointer>

#include <optional>

class QTimer;
class QTouchEvent;

using namespace flatland::lib;

class Window : public QWidget
{
public:
    Window(QSharedPointer<SimpleFlatland> simpleFlatland, QSharedPointer<AdvancedFlatland> advancedFlatland);

protected:
    QSize sizeHint() const override;
    bool event(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    void processOneTouchPoint(const QTouchEvent& event);
    void processTwoTouchPoints(const QTouchEvent& event);
    void toggleRenderArea();
    void changeSpeed(int delta);

    void updateScale(qreal scale, QPoint pt);
    void updateTopLeft(QPoint currentPoint, QPoint prevPoint);

private:
    AdjustableTimer m_timer;
    RenderAreaBase* m_simpleRenderArea;
    RenderAreaBase* m_advancedRenderArea;

    bool m_isActiveSimpleFlatland;

    QSize m_screenSize;
};
