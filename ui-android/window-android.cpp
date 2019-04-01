#include "window-android.h"

#include "ui/RenderAreaBase.h"

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

void Window::initTimer()
{
    m_timer->setTimerType(Qt::TimerType::PreciseTimer);
    m_timer->setInterval(sc_minTimerInterval);
    m_timer->callOnTimeout(
        [&]()
    {
        plotNextGeneration();
    });
    m_timer->start(sc_defaultTimerInterval);
}

Window::Window(QSharedPointer<SimpleFlatland> simpleFlatland, QSharedPointer<AdvancedFlatland> advancedFlatland)
    : QWidget(nullptr)
    , m_simpleFlatland(simpleFlatland)
    , m_simpleRenderArea(new RenderArea<SimpleFlatland>(this, simpleFlatland))
    , m_advancedFlatland(advancedFlatland)
    , m_advancedRenderArea(new RenderArea<AdvancedFlatland>(this, m_advancedFlatland))
    , m_isActiveSimpleFlatland(false)
    , m_timer(new QTimer(this))
{
    setEnabled(true);
    setWindowTitle("QT-Based Flatland");
    setAttribute(Qt::WA_AcceptTouchEvents);

    toggleRenderArea();
    initTimer();
}

QSize Window::sizeHint() const
{
    return m_isActiveSimpleFlatland ? m_simpleRenderArea->sizeHint() : m_advancedRenderArea->sizeHint();
}

void Window::toggleRenderArea()
{
    m_isActiveSimpleFlatland = !m_isActiveSimpleFlatland;
    if (m_isActiveSimpleFlatland)
    {
        m_advancedRenderArea->hide();
        m_simpleRenderArea->show();
    }
    else
    {
        m_advancedRenderArea->show();
        m_simpleRenderArea->hide();
    }
}

void Window::changeSpeed(int delta)
{
    const auto currentInterval = m_timer->interval();
    const auto interval = std::min(std::max(currentInterval - delta, sc_minTimerInterval), sc_maxTimerInterval);
    m_timer->setInterval(interval);
}

void Window::processOneTouchPoint(const QTouchEvent& event)
{
    enum class EAction
    {
        //eNone,
        eReposition,
        eToggleRenderArea,
        eChangeSpeed
    };

    auto decideAction = [&](const QPoint& currentPoint, const QPoint& prevPoint) -> EAction
    {
//        if (!m_renderAreaMoveStartPoint.has_value())
//            return EAction::eNone;
        if (std::abs(currentPoint.x() - prevPoint.x() <= currentPoint.x() * 0.05)
                && currentPoint.x() >= sizeHint().width() * 0.90)
        {
            return EAction::eChangeSpeed;
        }
        if (std::abs(currentPoint.y() - prevPoint.y() <= currentPoint.y() * 5. / 100)
                && currentPoint.y() >= sizeHint().height() * 0.90)
        {
            return EAction::eToggleRenderArea;
        }
        return EAction::eReposition;
    };

    const QPoint currentPoint(event.touchPoints().first().pos().toPoint());
    const QPoint prevPoint(event.touchPoints().first().lastPos().toPoint());
    if (event.type() == QEvent::TouchUpdate)
    {
        switch (decideAction(currentPoint, prevPoint))
        {
        case EAction::eReposition:
            updateTopLeft(currentPoint, prevPoint);
            break;
        case EAction::eToggleRenderArea:
            toggleRenderArea();
            break;
        case EAction::eChangeSpeed:
            changeSpeed(currentPoint.y() - prevPoint.y());
            break;
//        case EAction::eNone:
//            break;
        }
    }
}

void Window::processTwoTouchPoints(const QTouchEvent& event)
{
    const auto& touchPoints = event.touchPoints();
    const QTouchEvent::TouchPoint& touchPoint1 = touchPoints.first();
    const QTouchEvent::TouchPoint& touchPoint2 = touchPoints.last();

    const QLineF line1(touchPoint1.lastScenePos(), touchPoint2.lastScenePos());
    const QLineF line2(touchPoint1.scenePos(), touchPoint2.scenePos());

    updateScale(line2.length() / line1.length(), line2.center().toPoint());
}

bool Window::event(QEvent* event)
{
    switch (event->type())
    {
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd:
    {
        auto touchEvent = static_cast<QTouchEvent*>(event);
        switch (touchEvent->touchPoints().count())
        {
        case 1:
            processOneTouchPoint(*touchEvent);
            break;

        case 2:
            processTwoTouchPoints(*touchEvent);
            break;
        }

        break;
    }

    default:
        return QWidget::event(event);
    }

    return true;
}

void Window::updateScale(qreal scale, QPoint pt)
{
    if (m_isActiveSimpleFlatland)
        m_simpleRenderArea->updateScale(scale, pt);
    else
        m_advancedRenderArea->updateScale(scale, pt);
}

void Window::updateTopLeft(QPoint currentPoint, QPoint prevPoint)
{
    const QPoint ptNew(currentPoint.x() - prevPoint.x(), currentPoint.y() - prevPoint.y());
    if (m_isActiveSimpleFlatland)
        m_simpleRenderArea->updateTopLeft(ptNew);
    else
        m_advancedRenderArea->updateTopLeft(ptNew);
}

void Window::plotNextGeneration()
{
    if (m_isActiveSimpleFlatland)
    {
        m_simpleFlatland->Run();
        m_simpleRenderArea->update();
    }
    else
    {
        m_advancedFlatland->Run();
        m_advancedRenderArea->update();
    }
}
