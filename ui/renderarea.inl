#include "renderarea.h"

#include "ui_helpers.h"

#include <QPainter>
#include <QPen>

#include <vector>
#include <cmath>

namespace
{
    const double sc_minScale = 1;  // 1 cell takes area 1x1 px
    const double sc_maxScale = 16; // 1 cell takes area 16x16 px
    const double sc_defaultScale = sc_minScale;
}

template <typename TFlatland>
RenderArea<TFlatland>::RenderArea(QWidget *parent, QSharedPointer<TFlatland> flatland)
    : QWidget(parent)
    , m_size(QSize{ static_cast<int>(flatland->Width()), static_cast<int>(flatland->Height()) })
    , m_relativeTopLeftPoint{0, 0}
    , m_scale{sc_defaultScale}
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

    const QRect rcExt(0, 0, m_size.width(), m_size.height());
    painter.fillRect(rcExt, sc_externalAreaColor);

    const QRect rcBk(m_relativeTopLeftPoint, QSize(m_size.width() * m_scale, m_size.height() * m_scale));
    painter.fillRect(rcBk, sc_backgroundColor);

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

                const auto cellRelativeX = cellX * m_scale + m_relativeTopLeftPoint.x();
                const auto cellRelativeY = cellY * m_scale + m_relativeTopLeftPoint.y();

                if (cellRelativeX >= 0 && cellRelativeX < m_size.width() &&
                    cellRelativeY >= 0 && cellRelativeY < m_size.height())
                {
                    auto color = getColorOfCell(*m_flatland, i, j);
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
void RenderArea<TFlatland>::updateTopLeft(QPoint pt)
{
    m_relativeTopLeftPoint.setX(m_relativeTopLeftPoint.x() + pt.x());
    m_relativeTopLeftPoint.setY(m_relativeTopLeftPoint.y() + pt.y());
    update();
}

template <typename TFlatland>
void RenderArea<TFlatland>::updateScale(qreal scale, QPoint pt)
{
    const auto newScale = m_scale * scale;
    if (newScale >= sc_minScale && newScale <= sc_maxScale)
    {
        if (newScale > m_scale)
        {
            m_relativeTopLeftPoint.setX(2 * m_relativeTopLeftPoint.x() - pt.x());
            m_relativeTopLeftPoint.setY(2 * m_relativeTopLeftPoint.y() - pt.y());
        }
        else
        {
            m_relativeTopLeftPoint.setX((m_relativeTopLeftPoint.x() + pt.x()) / 2);
            m_relativeTopLeftPoint.setY((m_relativeTopLeftPoint.y() + pt.y()) / 2);
        }
        m_scale = newScale;
        update();
    }
}
