#include "window.h"

#include "renderarea.h"

#include <QTimer>
#include <QScreen>
#include <QWheelEvent>
#include <QDesktopWidget>

namespace
{

const int sc_minTimerInterval = 1;
const int sc_maxTimerInterval = 2000;
const int sc_defaultTimerInterval = 200;

const double sc_scaleRate = 2;

}

template <typename TFlatland>
Window<TFlatland>::Window(QSharedPointer<TFlatland> flatland)
    : QWidget(nullptr)
    , m_flatland(flatland)
    , m_renderArea(new RenderArea<TFlatland>(this, flatland))
    , m_timer(new QTimer(this))
{
    setEnabled(true);
    //setMouseTracking(true);
    setWindowTitle("QT-Based Flatland");

    m_timer->setTimerType(Qt::TimerType::PreciseTimer);
    m_timer->setInterval(sc_minTimerInterval);
    m_timer->callOnTimeout(
        [&]()
    {
        plotNextGeneration();
    });
    m_timer->start(sc_defaultTimerInterval);
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
    const auto currentInterval = m_timer->interval();
    const auto interval = std::min(std::max(currentInterval - delta, sc_minTimerInterval), sc_maxTimerInterval);
    m_timer->setInterval(interval);
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
        m_renderArea->updateTopLeft(ptNew);
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
    m_renderArea->updateScale(event->button() == Qt::MouseButton::LeftButton ? sc_scaleRate : 1. / sc_scaleRate,
                              event->pos());
    event->accept();
}

template <typename TFlatland>
void Window<TFlatland>::plotNextGeneration()
{
    m_flatland->Run();
    m_renderArea->update();
}
