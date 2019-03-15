#include "window.h"

#include "renderarea.h"

#include <QDebug>

#include <QTimer>
#include <QScreen>
#include <QWheelEvent>
#include <QApplication>
#include <QGuiApplication>
#include <QDesktopWidget>

namespace
{

const int sc_minTimerInterval = 1;
const int sc_maxTimerInterval = 1000;
const int sc_defaultTimerInterval = 200;

const size_t sc_defaultFlatlandWidth = 1000;
const size_t sc_defaultFlatlandHeight = 1000;

}

Window::Window(QWidget *parent)
    : QWidget(parent)
    , m_flatland(new flatland::lib::SimpleFlatland(flatland::lib::CreateSimpleMap(
        {{ sc_defaultFlatlandWidth, sc_defaultFlatlandHeight }})))
    , m_renderArea(new RenderArea(this, QPoint(0, 0), QGuiApplication::screens().front()->size(), 1, m_flatland))
    , m_timer(new QTimer(this))
{
    setWindowTitle("Flatland for QT");
    setWindowState(Qt::WindowFullScreen);

    m_timer->setTimerType(Qt::TimerType::PreciseTimer);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(plotNextGeneration()));
    m_timer->start(sc_defaultTimerInterval);

    setMouseTracking(true);
    setEnabled(true);
}

void Window::wheelEvent(QWheelEvent *event)
{
    const auto degrees = event->angleDelta() / 8 / 2;
    const auto delta = degrees.y();
    const auto currentInterval = m_timer->interval();
    const auto interval = std::min(std::max(currentInterval - delta, sc_minTimerInterval), sc_maxTimerInterval);
    m_timer->setInterval(interval);
    event->accept();
}

void Window::mousePressEvent(QMouseEvent* event)
{
    if (event->button() & Qt::LeftButton)
    {
        m_renderArea->updateScale(true);
        m_renderArea->updateTopLeft(event->x(), event->y());
        m_renderArea->update();
    }
    else if (event->button() & Qt::RightButton)
    {
        m_renderArea->updateScale(false);
        m_renderArea->updateTopLeft(event->x(), event->y());
        m_renderArea->update();
    }

    event->accept();
}

void Window::plotNextGeneration()
{
    m_flatland->Run();
    m_renderArea->update();
}
