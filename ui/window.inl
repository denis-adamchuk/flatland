#include "window.h"

#include "RenderAreaBase.h"
#include "RenderAreaFactory.h"
#include "AdjustableTimer.h"

#include <QWheelEvent>

template <typename TFlatland>
Window<TFlatland>::Window(QSharedPointer<TFlatland> flatland)
    : QWidget(nullptr)
    , m_timer(this,
                    [flatland, this]()
                {
                    flatland->Run();
                    m_renderArea->update();
                })
    , m_renderArea(CreateRenderArea(this, flatland))
{
    setEnabled(true);
    setWindowTitle("QT-Based Flatland");
}

template <typename TFlatland>
QSize Window<TFlatland>::sizeHint() const
{
    return m_renderArea->sizeHint();
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
    static const double sc_scaleRate = 2;

    m_renderArea->UpdateScale(event->button() == Qt::MouseButton::LeftButton ? sc_scaleRate : 1. / sc_scaleRate,
                              event->pos());
    event->accept();
}
