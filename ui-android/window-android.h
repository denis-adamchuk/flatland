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
    RenderAreaBase* m_simpleRenderArea = nullptr;
    RenderAreaBase* m_advancedRenderArea = nullptr;

    bool m_isActiveSimpleFlatland = false;

    QSize m_screenSize;

    qreal m_lineLength1;
    qreal m_lineLength2;
    std::optional<qreal> m_initialLength;
    qreal m_scale;

    int m_lasttype;
    int m_lastPointsCount;

    QPoint m_center1;
    QPoint m_center2;

    unsigned long m_prevScale = 1;
    qreal m_touchPointRescaleStep = 0;
};
