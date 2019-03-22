#pragma once

#include "renderarea.h"

#include <QWidget>
#include <QSharedPointer>

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
    void keyPressEvent(QKeyEvent* event) override;

private:
    QSharedPointer<TFlatland> m_flatland;
    RenderArea<TFlatland>* m_renderArea;
    QTimer* m_timer;
};

#include "window.inl"
