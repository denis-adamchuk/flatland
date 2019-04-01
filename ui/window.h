#pragma once

#include "TimerBasedFlatlandRunner.h"

#include <QWidget>
#include <QSharedPointer>

#include <optional>

struct RenderAreaBase;

template <typename TFlatland>
class Window : public QWidget
{
public:
    Window(QSharedPointer<TFlatland> flatland);

    QSize sizeHint() const override;

public:
    void plotNextGeneration();

protected:
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
    TimerBasedFlatlandRunner m_runner;
    RenderAreaBase* m_renderArea;
    std::optional<QPoint> m_renderAreaMoveStartPoint;
};

#include "window.inl"
