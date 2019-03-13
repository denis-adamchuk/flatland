#include "renderarea.h"

#include "lib/SimpleMapCreator.h"

#include <QPainter>
#include <QTimer>
#include <QPen>

#include <vector>

RenderArea::RenderArea(QWidget *parent, const QRect& rc)
    : QWidget(parent)
    , m_flatland(flatland::lib::CreateSimpleMap(
        {{ static_cast<size_t>(rc.width()), static_cast<size_t>(rc.height())}}))
{
    QTimer* timer = new QTimer(this);
    timer->setTimerType(Qt::TimerType::PreciseTimer);
    connect(timer, SIGNAL(timeout()), this, SLOT(plotNextGeneration()));
    timer->start(100);
}

QSize RenderArea::sizeHint() const
{
    return { static_cast<int>(m_flatland.Width()), static_cast<int>(m_flatland.Height()) };
}

void RenderArea::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);

    std::vector<QPoint> blackPoints;
    blackPoints.reserve(m_flatland.Width() * m_flatland.Height());

    std::vector<QPoint> whitePoints;
    whitePoints.reserve(m_flatland.Width() * m_flatland.Height());

    for (size_t i = 0; i < m_flatland.Width(); ++i)
    {
        for (size_t j = 0; j < m_flatland.Height(); ++j)
        {
            if (m_flatland.GetCell(i, j))
            {
                blackPoints.emplace_back(i, j);
            }
            else
            {
                whitePoints.emplace_back(i, j);
            }
        }
    }

    painter.setPen(Qt::black);
    painter.drawPoints(blackPoints.data(), static_cast<int>(blackPoints.size()));

    painter.setPen(Qt::white);
    painter.drawPoints(whitePoints.data(), static_cast<int>(whitePoints.size()));
}

void RenderArea::plotNextGeneration()
{
    m_flatland.Run();
    update();
}
