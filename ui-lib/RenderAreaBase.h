#pragma once

#include <QWidget>
#include <QSharedPointer>

namespace flatland
{
namespace lib
{
    struct IFlatland;
}
}

struct ScalingLimits
{
    unsigned long minScale;
    unsigned long maxScale;
    unsigned long defaultScale;
};

class RenderAreaBase : public QWidget
{
public:
    explicit RenderAreaBase(QWidget* parent, const ScalingLimits& scalingLimits);

    QSize sizeHint() const override;

    void UpdateTopLeft(QPoint pt);
    void Rescale(qreal scaleFactor, QPoint pt);
    unsigned long GetScale();

protected:
    void paintEvent(QPaintEvent* event) override;

protected:
    virtual const flatland::lib::IFlatland& getCells() const = 0;
    virtual QColor getColor(unsigned long i, unsigned long j) const = 0;

private:
    // dimensions of drawing area
    QSize m_size;

    // how distant is top left point of the screen from a cell with coordinates (0, 0)
    QPointF m_visibleAreaOffset;

    // current scale, number of pixels that match a single pixel in each of X/Y directions
    unsigned long m_scale;

    // min/max/default scale
    ScalingLimits m_scalingLimits;
};
