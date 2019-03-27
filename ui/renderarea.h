#pragma once

#include <QWidget>
#include <QSharedPointer>

template <typename TFlatland>
class RenderArea : public QWidget
{
public:
    explicit RenderArea(QWidget* parent, QSharedPointer<TFlatland> flatland);

    QSize sizeHint() const override;

    void updateTopLeft(QPoint pt);
    void updateScale(qreal scale, QPoint pt);

private:
    void doPaint();

private:
    // dimensions of drawing area
    QSize m_size;

    // how distant is cell with coordinates (0, 0) from the top left point of the screen
    QPoint m_relativeTopLeftPoint;

    // current scale
    double m_scale;

    // data
    QSharedPointer<TFlatland> m_flatland;

protected:
    void paintEvent(QPaintEvent* event) override;

};

#include "renderarea.inl"
