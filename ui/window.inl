#include "window.h"

#include "ui-lib/RenderAreaBase.h"
#include "ui-lib/RenderAreaFactory.h"
#include "ui-lib/AdjustableTimer.h"

#include <QScreen>
#include <QGuiApplication>
#include <QWheelEvent>

namespace
{
    const unsigned long sc_minScale = 1;  // 1 cell takes area 1x1 px
    const unsigned long sc_maxScale = 64; // 1 cell takes area 64x64 px
    const unsigned long sc_defaultScale = 8;
    const unsigned long sc_scaleFactor = 2;
}

template <typename TFlatland>
Window<TFlatland>::Window(QSharedPointer<TFlatland> flatland)
    : QWidget(nullptr)
    , m_timer(this,
                    [flatland, this]()
                {
                    flatland->Run();
                    m_renderArea->update();
                })
    , m_renderArea(CreateRenderArea(this, flatland, {sc_minScale, sc_maxScale, sc_defaultScale}))
{
    setEnabled(true);
    setWindowTitle("QT-Based Flatland");
}

template <typename TFlatland>
QSize Window<TFlatland>::sizeHint() const
{
    return QGuiApplication::screens().front()->size();
}

template <typename TFlatland>
void Window<TFlatland>::wheelEvent(QWheelEvent *event)
{
    const auto degrees = event->angleDelta() / 8 / 2;
    const auto delta = degrees.y();
    m_timer.AdjustInterval(delta);
    event->accept();
}

template <typename TFlatland>
void Window<TFlatland>::mousePressEvent(QMouseEvent* event)
{
    m_renderAreaMoveStartPoint = { event->x(), event->y() };
    event->accept();
}

template <typename TFlatland>
void Window<TFlatland>::mouseMoveEvent(QMouseEvent* event)
{
    if (m_renderAreaMoveStartPoint.has_value())
    {
        const QPoint pt(event->pos());
        const QPoint ptNew(pt.x() - m_renderAreaMoveStartPoint->x(),
                           pt.y() - m_renderAreaMoveStartPoint->y());
        m_renderArea->UpdateTopLeft(ptNew);
        m_renderAreaMoveStartPoint = pt;
    }
    event->accept();
}

template <typename TFlatland>
void Window<TFlatland>::mouseReleaseEvent(QMouseEvent* event)
{
    m_renderAreaMoveStartPoint.reset();
    event->accept();
}

template <typename TFlatland>
void Window<TFlatland>::mouseDoubleClickEvent(QMouseEvent *event)
{
    const auto scaleFactor = event->button() == Qt::MouseButton::LeftButton ? sc_scaleFactor : 1. / sc_scaleFactor;
//    const auto scaleFactor = event->button() == Qt::MouseButton::LeftButton
//            ? static_cast<double>(m_renderArea->GetScale() + 1) / m_renderArea->GetScale()
//            : 1. / (static_cast<double>(m_renderArea->GetScale() + 1) / m_renderArea->GetScale());
    m_renderArea->Rescale(scaleFactor, event->pos());
    event->accept();
}
