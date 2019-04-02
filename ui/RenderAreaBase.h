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

class RenderAreaBase : public QWidget
{
public:
    explicit RenderAreaBase(QWidget* parent);

    QSize sizeHint() const override;

    void UpdateTopLeft(QPoint pt);
    void UpdateScale(qreal scale, QPoint pt);

protected:
    void paintEvent(QPaintEvent* event) override;

protected:
    virtual const flatland::lib::IFlatland& getCells() const = 0;
    virtual QColor getColor(unsigned long i, unsigned long j) const = 0;

private:
    // dimensions of drawing area
    QSize m_size;

    // how distant is cell with coordinates (0, 0) from the top left point of the screen
    QPoint m_relativeTopLeftPoint;

    // current scale, number of pixels that match a single pixel in each of X/Y directions
    unsigned long m_scale;
};
