#include "window-android.h"

#include "ui-lib/RenderAreaFactory.h"

#include <QScreen>
#include <QPainter>
#include <QWheelEvent>
#include <QGuiApplication>

#include <cmath>

Window::Window(QSharedPointer<SimpleFlatland> simpleFlatland,
               QSharedPointer<AdvancedFlatland> advancedFlatland,
               unsigned long defaultScale)
    : QWidget(nullptr)
    , m_timer(this,
            [=]()
        {
            if (m_isActiveSimpleFlatland)
            {
                simpleFlatland->Run();
                m_simpleRenderArea->update();
            }
            else
            {
                advancedFlatland->Run();
                m_advancedRenderArea->update();
            }
        })
{
    setEnabled(true);
    setAttribute(Qt::WA_AcceptTouchEvents);

    ScalingLimits limits{defaultScale, defaultScale * 10, defaultScale};
    m_simpleRenderArea = CreateRenderArea(this, simpleFlatland, limits);
    m_advancedRenderArea = CreateRenderArea(this, advancedFlatland, limits);
    toggleRenderArea();
}

QSize Window::sizeHint() const
{
    // Full-screen
    return QGuiApplication::screens().front()->size();
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
    m_timer.AdjustInterval(-delta);
    update();
}

void Window::processOneTouchPoint(const QTouchEvent& event)
{
    enum class EAction
    {
        eReposition,
        eToggleRenderArea,
        eChangeSpeed
    };

    auto checkDeviation = [&](const QPoint& pt1, const QPoint& pt2, bool x, qreal limit)
    {
        return x ? std::abs(pt1.x() - pt2.x()) <= (pt1.x() * limit)
                 : std::abs(pt1.y() - pt2.y()) <= (pt1.y() * limit);
    };

    auto decideAction = [&](const QPoint& currentPoint, const QPoint& prevPoint) -> EAction
    {
        if (checkDeviation(currentPoint, prevPoint, true, 0.10) && currentPoint.x() >= sizeHint().width() * 0.80)
            return EAction::eChangeSpeed;
        if (checkDeviation(currentPoint, prevPoint, false, 0.10) && currentPoint.y() >= sizeHint().height() * 0.80)
            return EAction::eToggleRenderArea;
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

    const auto currentScale = m_isActiveSimpleFlatland  ? m_simpleRenderArea->GetScale()
                                                        : m_advancedRenderArea->GetScale();

    if (!m_initialLength)
    {
        m_initialLength = line2.length();
        m_initialScale = currentScale;
    }
    else
    {
        const auto rescale = line2.length() / *m_initialLength;
        const auto adjusted = rescale / (static_cast<qreal>(currentScale) / *m_initialScale);
        const auto pt = line2.center().toPoint();
        updateScale(adjusted, pt);
    }

    update();
}

bool Window::event(QEvent* event)
{
    switch (event->type())
    {
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd:
    {
        if (event->type() == QEvent::TouchEnd)
        {
            m_initialLength.reset();
            m_initialScale.reset();
        }
        const auto touchEvent = static_cast<QTouchEvent*>(event);
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
        m_simpleRenderArea->Rescale(scale, pt);
    else
        m_advancedRenderArea->Rescale(scale, pt);
}

void Window::updateTopLeft(QPoint currentPoint, QPoint prevPoint)
{
    const QPoint ptNew(currentPoint.x() - prevPoint.x(), currentPoint.y() - prevPoint.y());
    if (m_isActiveSimpleFlatland)
        m_simpleRenderArea->UpdateTopLeft(ptNew);
    else
        m_advancedRenderArea->UpdateTopLeft(ptNew);
}
