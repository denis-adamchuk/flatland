#include "RenderAreaBase.h"

#include "lib/FlatlandItf.h"

#include <QString>
#include <QPainter>
#include <QPen>

#include <vector>
#include <cmath>

namespace
{
    const QRgb sc_backgroundColor = qRgb(250, 250, 215);
    const QRgb sc_externalAreaColor = qRgb(200, 200, 200);

    std::vector<QString> GetLegend(const flatland::lib::IFlatland& flatland)
    {
        std::vector<QString> v;

        for (const auto& i: flatland.GetStatistics())
        {
            const auto& name = i.first;
            const auto& val = i.second;
            v.push_back(QString("%1: %2").arg(name.c_str(), val.GetAsText().c_str()));
        }

        return v;
    }
}

RenderAreaBase::RenderAreaBase(QWidget *parent, const ScalingLimits& scalingLimits)
    : QWidget(parent)
    , m_scale(scalingLimits.defaultScale)
    , m_scalingLimits(scalingLimits)
{
}

QSize RenderAreaBase::sizeHint() const
{
    return parentWidget()->sizeHint();
}

void RenderAreaBase::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);

    QFont font = painter.font();
    font.setPixelSize(10);
    painter.setFont(font);

    const QSize& size = sizeHint();
    const QRect rcExt(0, 0, size.width(), size.height());
    painter.fillRect(rcExt, sc_externalAreaColor);

    const auto& cells = getCells();
    const QPointF offsetTopLeft{ -m_visibleAreaOffset.x(), -m_visibleAreaOffset.y() };
    const QRectF rcBk(offsetTopLeft, QSize(static_cast<int>(cells.Width() * m_scale),
                                           static_cast<int>(cells.Height() * m_scale)));
    painter.fillRect(rcBk, sc_backgroundColor);

    painter.setPen(Qt::red);
    int ypos = 20; // start
    for (const auto& s : GetLegend(cells))
    {
        painter.drawText(QPoint{10, ypos}, s);
        ypos += 20;
    }

    std::optional<QColor> prevColor;
    for (size_t j = 0; j < cells.Height(); ++j)
    {
        for (size_t i = 0; i < cells.Width(); ++i)
        {
            if (cells.IsCellAlive(i, j))
            {
                const auto cellX = static_cast<int>(i);
                const auto cellY = static_cast<int>(j);

                const auto cellRelativeX = cellX * static_cast<int>(m_scale) - m_visibleAreaOffset.x();
                const auto cellRelativeY = cellY * static_cast<int>(m_scale) - m_visibleAreaOffset.y();

                if (cellRelativeX >= 0 && cellRelativeX < size.width() &&
                    cellRelativeY >= 0 && cellRelativeY < size.height())
                {
                    const auto color = getColor(i, j);

//                    if (i < 100 || i > size.width()-100)
//                        color = Qt::blue;
//                    else if (i > size.width() / 2 - 50 && i < size.width() / 2 +50 &&
//                             j > size.height() / 2 - 50 && j < size.height() / 2 + 50)
//                        color = Qt::red;
//                    painter.setPen(color);

                    if (m_scale == 1)
                    {
                        if (!prevColor || *prevColor != color)
                            painter.setPen(color);
                        prevColor = color;

                        for (size_t sX = 0; sX < m_scale; ++sX)
                        {
                            for (size_t sY = 0; sY < m_scale; ++sY)
                            {
                                painter.drawPoint(QPointF{cellRelativeX + static_cast<int>(sX),
                                                          cellRelativeY + static_cast<int>(sY)});
                            }
                        }
                    }
                    else if (m_scale < 8)
                    {
                        QRectF rcCell(cellRelativeX, cellRelativeY, m_scale, m_scale);
                        painter.fillRect(rcCell, color);
                    }
                    else
                    {
                        QRectF rcCell(cellRelativeX + 1, cellRelativeY + 1, m_scale - 2, m_scale - 2);
                        painter.fillRect(rcCell, color);

                        painter.setPen(Qt::lightGray);
                        painter.drawRect(rcCell);
                    }
                }
            }
        }
    }
}

void RenderAreaBase::UpdateTopLeft(QPoint pt)
{
    m_visibleAreaOffset.setX(m_visibleAreaOffset.x() - pt.x());
    m_visibleAreaOffset.setY(m_visibleAreaOffset.y() - pt.y());
    update();
}

void RenderAreaBase::Rescale(qreal scaleFactor, QPoint pt)
{
    const auto intNewScale = static_cast<unsigned long>(m_scale * scaleFactor);
    if (intNewScale != m_scale && intNewScale >= m_scalingLimits.minScale && intNewScale <= m_scalingLimits.maxScale)
    {
        const auto intScaleFactor = static_cast<qreal>(intNewScale) / m_scale;
        m_visibleAreaOffset = { intScaleFactor * (m_visibleAreaOffset.x() + pt.x()) - pt.x(),
                                intScaleFactor * (m_visibleAreaOffset.y() + pt.y()) - pt.y() };
        m_scale = intNewScale;
        update();
    }
}

unsigned long RenderAreaBase::GetScale()
{
    return m_scale;
}
