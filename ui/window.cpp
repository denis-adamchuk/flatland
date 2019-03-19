#include "window.h"

#include "renderarea.h"

#include <QDebug>
#include <QElapsedTimer>

#include <QTimer>
#include <QScreen>
#include <QWheelEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QGuiApplication>

namespace
{

const int sc_minTimerInterval = 1;
const int sc_maxTimerInterval = 1000;
const int sc_defaultTimerInterval = 200;

}

Window::Window(QWidget *parent, unsigned long width, unsigned long height,
    unsigned long maxAge, unsigned long maxReproductivityAge)
    : QWidget(parent)
    , m_flatland(new flatland::lib::AdvancedFlatland(flatland::lib::CreateAdvancedMap(
        {{ width, height}}), maxAge, maxReproductivityAge))
    , m_renderArea(new RenderArea(this, QPoint(0, 0),
        QSize{ static_cast<int>(width), static_cast<int>(height) }, 1, m_flatland))
    , m_timer(new QTimer(this))
{
    setWindowTitle("Flatland for QT");

    if (width == static_cast<size_t>(QGuiApplication::screens().front()->size().width()) &&
        height == static_cast<size_t>(QGuiApplication::screens().front()->size().height()))
    {
        setWindowState(Qt::WindowFullScreen);
    }

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
        //m_renderArea->updateScale(true);
        m_renderArea->updateTopLeft(event->x(), event->y());
        m_renderArea->update();
    }
    else if (event->button() & Qt::RightButton)
    {
        //m_renderArea->updateScale(false);
        m_renderArea->updateTopLeft(event->x(), event->y());
        m_renderArea->update();
    }

    event->accept();
}

void Window::keyPressEvent(QKeyEvent *event)
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

void Window::plotNextGeneration()
{
    m_flatland->Run();
    m_renderArea->update();
}
