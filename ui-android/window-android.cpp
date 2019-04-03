#include "window-android.h"

#include "ui/RenderAreaFactory.h"

#include <QScreen>
#include <QPainter>
#include <QWheelEvent>
#include <QGuiApplication>

Window::Window(QSharedPointer<SimpleFlatland> simpleFlatland, QSharedPointer<AdvancedFlatland> advancedFlatland)
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
    , m_simpleRenderArea(CreateRenderArea(this, simpleFlatland))
    , m_advancedRenderArea(CreateRenderArea(this, advancedFlatland))
    , m_isActiveSimpleFlatland(false)
{
    setAttribute(Qt::WA_AcceptTouchEvents);

    toggleRenderArea();

    m_screenSize = QGuiApplication::screens().front()->size();
}

QSize Window::sizeHint() const
{
    return m_screenSize;
    //return m_isActiveSimpleFlatland ? m_simpleRenderArea->sizeHint() : m_advancedRenderArea->sizeHint();
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
    m_timer.AdjustInterval(delta);
    update();
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

    auto checkDeviation = [&](const QPoint& pt1, const QPoint& pt2, bool x, qreal limit)
    {
        return x ? std::abs(pt1.x() - pt2.x()) <= (pt1.x() * limit)
                 : std::abs(pt1.y() - pt2.y()) <= (pt1.y() * limit);
    };

    auto decideAction = [&](const QPoint& currentPoint, const QPoint& prevPoint) -> EAction
    {
//        if (!m_renderAreaMoveStartPoint.has_value())
//            return EAction::eNone;
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

    //updateScale(line2.length() / line1.length(), line2.center().toPoint());
    updateScale(2.0, line2.center().toPoint());
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

void Window::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);

    QFont font = painter.font();
    font.setPixelSize(12);
    painter.setFont(font);

    painter.setPen(Qt::blue);
    painter.drawText(QPoint{m_screenSize.width() / 2, m_screenSize.height()},
                     QString("Speed: %1").arg(QString::number(m_timer.GetInterval())));
}
