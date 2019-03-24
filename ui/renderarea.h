#pragma once

#include <QWidget>
#include <QSharedPointer>

template <typename TFlatland>
class RenderArea : public QWidget
{
public:
    explicit RenderArea(QWidget* parent, const QSize size, QSharedPointer<TFlatland> flatland);

    QSize sizeHint() const override;

    void updateTopLeft(int x, int y);
    void updateScale(bool increment, int x, int y);

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
