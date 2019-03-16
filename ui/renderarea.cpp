#include "renderarea.h"

#include "lib/AdvancedMapCreator.h"

#include <QDebug>
#include <QElapsedTimer>

#include <QPainter>
#include <QPen>

#include <vector>
#include <iostream>
#include <fstream>

namespace
{
    const QRgb sc_backgroundColor = qRgb(250, 250, 215);

    QColor getColorByAge(unsigned long age, unsigned long maxAge)
    {
        int h = 0;
        int s = 0;
        int v = 0;

             if (age > maxAge * 0.0 && age < maxAge * 0.2)
            h = 120; // G
        else if (age > maxAge * 0.2 && age < maxAge * 0.4)
            h = 30;  // Orange
        else if (age > maxAge * 0.4 && age < maxAge * 0.6)
            h = 0;   // R
        else if (age > maxAge * 0.6 && age < maxAge * 0.8)
            h = 300; // Magenta
        else if (age > maxAge * 0.8 && age < maxAge * 1.0)
            h = 240; // B
        const auto ageTo255 = static_cast<int>(255. * age / maxAge);
        static const auto lowestV = 130;
        s = v = std::max(lowestV, 255 - ageTo255);

        return QColor::fromHsv(h, s, v);
    }
}

RenderArea::RenderArea(QWidget *parent, const QPoint& topLeft, const QSize size, int scale,
                       QSharedPointer<flatland::lib::AdvancedFlatland> flatland)
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

void RenderArea::doPaint()
{
    QPainter painter(this);

    const QRect rc(0, 0, m_size.width(), m_size.height());
    painter.fillRect(rc, sc_backgroundColor);

    const auto& stat = m_flatland->GetStatistics();

    painter.setPen(Qt::red);
    painter.drawText(QPoint{10, 20}, QString("generation: %1").
        arg(stat._generation));
    painter.drawText(QPoint{10, 40}, QString("alive cells: %1 (%2%3)").
        arg(QString::number(stat._aliveCells),
            (stat._aliveCellsDelta > 0 ? "+" : "-"), QString::number(stat._aliveCellsDelta)));
    painter.drawText(QPoint{10, 60}, QString("reproductive cells: %1 (%2%3)").
        arg(QString::number(stat._reproductiveCells),
            (stat._reproductiveCellsDelta > 0 ? "+" : "-"), QString::number(stat._reproductiveCellsDelta)));

    for (size_t j = 0; j < m_flatland->Height(); ++j)
    {
        for (size_t i = 0; i < m_flatland->Width(); ++i)
        {
            const auto& age = m_flatland->GetCell(i, j)._age;
            if (age > 0)
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
                    const auto color = getColorByAge(age, m_flatland->GetMaxAge());
                    painter.setPen(color);
                    for (int sX = 0; sX < m_scale; ++sX)
                    {
                        for (int sY = 0; sY < m_scale; ++sY)
                        {
                            painter.drawPoint(scaledCellX + sX, scaledCellY + sY);
                        }
                    }
                }
            }
        }
    }
}

void RenderArea::paintEvent(QPaintEvent * /* event */)
{
    doPaint();
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
    if (increment && m_scale < 32)
    {
        m_scale *= 2;
    }
    else if (!increment && m_scale > 1)
    {
        m_scale /= 2;
    }
}
