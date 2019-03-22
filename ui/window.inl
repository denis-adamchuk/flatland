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

}

template <typename TFlatland>
Window<TFlatland>::Window(QSharedPointer<TFlatland> flatland)
    : QWidget(nullptr)
    , m_flatland(flatland)
    , m_renderArea(new RenderArea<TFlatland>(
        this,
        QPoint(0, 0),
        QSize{ static_cast<int>(flatland->Width()), static_cast<int>(flatland->Height()) },
        1.0,
        flatland))
    , m_timer(new QTimer(this))
{
    setEnabled(true);
    setMouseTracking(true);
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
    if (event->button() & Qt::LeftButton)
    {
        m_renderArea->updateTopLeft(event->x(), event->y());
        m_renderArea->update();
    }
    else if (event->button() & Qt::RightButton)
    {
        m_renderArea->updateTopLeft(event->x(), event->y());
        m_renderArea->update();
    }

    event->accept();
}

template <typename TFlatland>
void Window<TFlatland>::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key::Key_Plus)
    {
        m_renderArea->updateScale(true);
    }
    else if (event->key() == Qt::Key::Key_Minus)
    {
        m_renderArea->updateScale(false);
    }

    event->accept();
}

template <typename TFlatland>
void Window<TFlatland>::plotNextGeneration()
{
    m_flatland->Run();
    m_renderArea->update();
}
