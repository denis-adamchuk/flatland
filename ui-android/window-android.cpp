#include "window-android.h"

#include "ui/RenderAreaFactory.h"

#include <QWheelEvent>

Window::Window(QSharedPointer<SimpleFlatland> simpleFlatland, QSharedPointer<AdvancedFlatland> advancedFlatland)
    : QWidget(nullptr)
    , m_runner(this,
                    [&]()
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
    , m_simpleRenderArea(CreateRenderArea(this, simpleFlatland))
    , m_advancedRenderArea(CreateRenderArea(this, advancedFlatland))
{
    setAttribute(Qt::WA_AcceptTouchEvents);

    toggleRenderArea();
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
    m_runner.AdjustInterval(delta);
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
        m_simpleRenderArea->UpdateScale(scale, pt);
    else
        m_advancedRenderArea->UpdateScale(scale, pt);
}

void Window::updateTopLeft(QPoint currentPoint, QPoint prevPoint)
{
    const QPoint ptNew(currentPoint.x() - prevPoint.x(), currentPoint.y() - prevPoint.y());
    if (m_isActiveSimpleFlatland)
        m_simpleRenderArea->UpdateTopLeft(ptNew);
    else
        m_advancedRenderArea->UpdateTopLeft(ptNew);
}
