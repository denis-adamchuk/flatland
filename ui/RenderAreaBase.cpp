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

    const double sc_minScale = 1;  // 1 cell takes area 1x1 px
    const double sc_maxScale = 16; // 1 cell takes area 16x16 px
    const double sc_defaultScale = sc_minScale;

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

RenderAreaBase::RenderAreaBase(QWidget *parent)
    : QWidget(parent)
    , m_relativeTopLeftPoint{0, 0}
    , m_scale{sc_defaultScale}
{
}

QSize RenderAreaBase::sizeHint() const
{
    const auto& cells = getCells();
    return { static_cast<int>(cells.Width()), static_cast<int>(cells.Height()) };
}

void RenderAreaBase::paintEvent(QPaintEvent * /* event */)
{
    const auto& cells = getCells();
    const QSize& size = sizeHint();

    QPainter painter(this);

    const QRect rcExt(0, 0, size.width(), size.height());
    painter.fillRect(rcExt, sc_externalAreaColor);

    const QRect rcBk(m_relativeTopLeftPoint, QSize(size.width() * m_scale, size.height() * m_scale));
    painter.fillRect(rcBk, sc_backgroundColor);

    painter.setPen(Qt::red);
    int ypos = 20; // start
    for (const auto& s : GetLegend(cells))
    {
        painter.drawText(QPoint{10, ypos}, s);
        ypos += 20;
    }

    for (size_t j = 0; j < size.height(); ++j)
    {
        for (size_t i = 0; i < size.width(); ++i)
        {
            if (cells.IsCellAlive(i, j))
            {
                const auto cellX = static_cast<int>(i);
                const auto cellY = static_cast<int>(j);

                const auto cellRelativeX = cellX * m_scale + m_relativeTopLeftPoint.x();
                const auto cellRelativeY = cellY * m_scale + m_relativeTopLeftPoint.y();

                if (cellRelativeX >= 0 && cellRelativeX < size.width() &&
                    cellRelativeY >= 0 && cellRelativeY < size.height())
                {
                    painter.setPen(getColor(i, j));
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

void RenderAreaBase::UpdateTopLeft(QPoint pt)
{
    m_relativeTopLeftPoint.setX(m_relativeTopLeftPoint.x() + pt.x());
    m_relativeTopLeftPoint.setY(m_relativeTopLeftPoint.y() + pt.y());
    update();
}

void RenderAreaBase::UpdateScale(qreal scale, QPoint pt)
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
