#pragma once

#include "ui/renderarea.h"
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

    QSize sizeHint() const override;

public:
    void plotNextGeneration();

protected:
    bool event(QEvent* event) override;

private:
    void processOneTouchPoint(const QTouchEvent& event);
    void processTwoTouchPoints(const QTouchEvent& event);
    void toggleRenderArea();
    void changeSpeed(int delta);

    void updateScale(qreal scale, QPoint pt);
    void updateTopLeft(QPoint currentPoint, QPoint prevPoint);

private:
    QSharedPointer<SimpleFlatland> m_simpleFlatland;
    RenderArea<SimpleFlatland>* m_simpleRenderArea;

    QSharedPointer<AdvancedFlatland> m_advancedFlatland;
    RenderArea<AdvancedFlatland>* m_advancedRenderArea;

    bool m_isActiveSimpleFlatland;

    QTimer* m_timer;
    //std::optional<QPoint> m_renderAreaMoveStartPoint;
    void initTimer();
};
