#include "renderarea.h"

#include "ui_helpers.h"

#include <QPainter>
#include <QPen>

#include <vector>

namespace
{
}

template <typename TFlatland>
RenderArea<TFlatland>::RenderArea(QWidget *parent, const QPoint& topLeft, const QSize size, int scale,
                       QSharedPointer<TFlatland> flatland)
    : QWidget(parent)
    , m_size(size)
    , m_relativeTopLeftPoint(topLeft)
    , m_scale(scale)
    , m_flatland(flatland)
{
}

template <typename TFlatland>
QSize RenderArea<TFlatland>::sizeHint() const
{
    return m_size;
}

template <typename TFlatland>
void RenderArea<TFlatland>::doPaint()
{
    QPainter painter(this);

    const QRect rc(0, 0, m_size.width(), m_size.height());
    painter.fillRect(rc, sc_backgroundColor);

    painter.setPen(Qt::red);
    int ypos = 20; // start
    for (const auto& s : getLegend(*m_flatland))
    {
        painter.drawText(QPoint{10, ypos}, s);
        ypos += 20;
    }

    for (size_t j = 0; j < m_flatland->Height(); ++j)
    {
        for (size_t i = 0; i < m_flatland->Width(); ++i)
        {
            if (m_flatland->IsCellAlive(i, j))
            {
                const auto cellX = static_cast<int>(i);
                const auto cellY = static_cast<int>(j);

                const auto cellRelativeX = cellX * m_scale - m_relativeTopLeftPoint.x();
                const auto cellRelativeY = cellY * m_scale - m_relativeTopLeftPoint.y();

                if (cellRelativeX >= 0 && cellRelativeX < m_size.width() &&
                    cellRelativeY >= 0 && cellRelativeY < m_size.height())
                {
                    auto color = getColorOfCell(*m_flatland, i, j);
                    if (i < 50)
                        color = Qt::red;
                    else if (i > m_flatland->Width() - 50)
                        color = Qt::blue;
                    painter.setPen(color);
                    for (int sX = 0; sX < m_scale; ++sX)
                    {
                        for (int sY = 0; sY < m_scale; ++sY)
                        {
                            painter.drawPoint(cellRelativeX + sX, cellRelativeY + sY);
                        }
                    }
                }
            }
        }
    }
}

template <typename TFlatland>
void RenderArea<TFlatland>::paintEvent(QPaintEvent * /* event */)
{
    doPaint();
}

template <typename TFlatland>
void RenderArea<TFlatland>::updateTopLeft(int x, int y)
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

template <typename TFlatland>
void RenderArea<TFlatland>::updateScale(bool increment)
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
