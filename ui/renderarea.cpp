#include "renderarea.h"

#include "lib/SimpleMapCreator.h"

#include <QPainter>
#include <QPen>

#include <vector>

RenderArea::RenderArea(QWidget *parent, const QPoint& topLeft, const QSize size, int scale,
                       QSharedPointer<flatland::lib::SimpleFlatland> flatland)
    : QWidget(parent)
    , m_size(size)
    , m_relativeTopLeftPoint(topLeft)
    , m_scale(scale)
    , m_flatland(flatland)
{
}

QSize RenderArea::sizeHint() const
{
    return m_size;
}

void RenderArea::paintEvent(QPaintEvent * /* event */)
{
    std::vector<QPoint> blackPoints;
    blackPoints.reserve(static_cast<size_t>(m_size.width() * m_size.height()));

    for (size_t i = 0; i < m_flatland->Width(); ++i)
    {
        for (size_t j = 0; j < m_flatland->Height(); ++j)
        {
            if (m_flatland->GetCell(i, j))
            {
                const auto cellX = static_cast<int>(i);
                const auto cellY = static_cast<int>(j);

                const auto cellRelativeX = cellX - m_relativeTopLeftPoint.x();
                const auto cellRelativeY = cellY - m_relativeTopLeftPoint.y();

                const auto scaledCellX = cellRelativeX * m_scale;
                const auto scaledCellY = cellRelativeY * m_scale;

                if (scaledCellX >= 0 && scaledCellX < m_size.width() &&
                    scaledCellY >= 0 && scaledCellY < m_size.height())
                {
                    for (int sX = 0; sX < m_scale; ++sX)
                    {
                        for (int sY = 0; sY < m_scale; ++sY)
                        {
                            blackPoints.emplace_back(scaledCellX + sX, scaledCellY + sY);
                        }
                    }
                }
            }
        }
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::RenderHint::Antialiasing);

    const QRect rc(0, 0, m_size.width(), m_size.height());
    painter.fillRect(rc, Qt::white);

    painter.setPen(Qt::black);
    painter.drawPoints(blackPoints.data(), static_cast<int>(blackPoints.size()));
}

void RenderArea::updateTopLeft(int x, int y)
{
    const auto iClicked = (m_relativeTopLeftPoint.x() + x) / m_scale;
    const auto jClicked = (m_relativeTopLeftPoint.y() + y) / m_scale;

    const auto iCenter = (m_size.width() / 2) / m_scale;
    const auto jCenter = (m_size.height() / 2) / m_scale;

    const auto iOffset = iCenter - iClicked;
    const auto jOffset = jCenter - jClicked;

    m_relativeTopLeftPoint.setX(-iOffset);
    m_relativeTopLeftPoint.setY(-jOffset);
}

void RenderArea::updateScale(bool increment)
{
    if (increment && m_scale < 16)
    {
        m_scale *= 2;
    }
    else if (!increment && m_scale > 1)
    {
        m_scale /= 2;
    }
}
