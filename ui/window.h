#pragma once

#include "ui-lib/AdjustableTimer.h"

#include <QWidget>
#include <QSharedPointer>

#include <optional>

struct RenderAreaBase;

// Main window for Desktop UI version of Flatland application
template <typename TFlatland>
class Window : public QWidget
{
public:
    Window(QSharedPointer<TFlatland> flatland);

protected:
    QSize sizeHint() const override;
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
    AdjustableTimer m_timer;
    RenderAreaBase* m_renderArea;
    std::optional<QPoint> m_renderAreaMoveStartPoint;
};

#include "window.inl"
