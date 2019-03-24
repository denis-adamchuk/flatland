#pragma once

#include "renderarea.h"

#include <QWidget>
#include <QSharedPointer>

#include <optional>

class QTimer;

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
    QSharedPointer<TFlatland> m_flatland;
    RenderArea<TFlatland>* m_renderArea;
    QTimer* m_timer;
    std::optional<QPoint> m_renderAreaMoveStartPoint;
};

#include "window.inl"
