#include "window-android.h"

#include "ui-lib/RenderAreaFactory.h"

#include <QScreen>
#include <QPainter>
#include <QWheelEvent>
#include <QGuiApplication>

#include <cmath>

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
{
    setEnabled(true);
    setAttribute(Qt::WA_AcceptTouchEvents);

    toggleRenderArea();

    m_screenSize = QGuiApplication::screens().front()->size();
    m_touchPointRescaleStep = std::sqrt(
                m_screenSize.width() * m_screenSize.width() + m_screenSize.height() * m_screenSize.height()) / 100.;
}

QSize Window::sizeHint() const
{
    return m_screenSize;
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

    const auto pt = line2.center().toPoint();

    if (!m_initialLength)
    {
        m_initialLength = line2.length();
    }
    else
    {
		const auto prevDelta = line1.length() - *m_initialLength;
        const auto curDelta = line2.length() - *m_initialLength;
        if (delta > 0)
        {
			const auto rescale = (curDelta - prevDelta) / m_touchPointRescaleStep;
            updateScale(rescale, pt);
        }
        else if (delta < 0)
        {
			const auto rescale = m_touchPointRescaleStep / (curDelta - prevDelta);
			updateScale(rescale, pt);
        }
    }

    m_lineLength1 = line1.length();
    m_lineLength2 = line2.length();
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
        }
        m_lasttype = event->type();
        auto touchEvent = static_cast<QTouchEvent*>(event);
        m_lastPointsCount = touchEvent->touchPoints().count();
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
    m_scale = scale;
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

    int ypos = m_screenSize.height() - 180;
    painter.setPen(Qt::blue);
    painter.drawText(QPoint{m_screenSize.width() / 2, ypos},
                     QString("Speed: %1").arg(QString::number(m_timer.GetInterval())));
    ypos += 20;
    painter.drawText(QPoint{m_screenSize.width() / 2, ypos},
                     QString("Line Length 1: %1").arg(QString::number(m_lineLength1)));
    ypos += 20;
    painter.drawText(QPoint{m_screenSize.width() / 2, ypos},
                     QString("Line Length 2: %1").arg(QString::number(m_lineLength2)));
    ypos += 20;
    painter.drawText(QPoint{m_screenSize.width() / 2, ypos},
                     QString("m_scale: %1").arg(QString::number(m_scale)));
    ypos += 20;
    painter.drawText(QPoint{m_screenSize.width() / 2, ypos},
                     QString("Initial Length: %1").arg(QString::number(*m_initialLength)));
    ypos += 20;
    painter.drawText(QPoint{m_screenSize.width() / 2, ypos},
                     QString("Last type: %1").arg(QString::number(m_lasttype)));
    ypos += 20;
    painter.drawText(QPoint{m_screenSize.width() / 2, ypos},
                     QString("Last points count: %1").arg(QString::number(m_lastPointsCount)));
    ypos += 20;
    painter.drawText(QPoint{m_screenSize.width() / 2, ypos},
                     QString("Center1: {%1;%2}").arg(QString::number(m_center1.x()),
                                                     QString::number(m_center1.y())));
    ypos += 20;
    painter.drawText(QPoint{m_screenSize.width() / 2, ypos},
                     QString("Center2: {%1;%2}").arg(QString::number(m_center2.x()),
                                                     QString::number(m_center2.y())));
}
