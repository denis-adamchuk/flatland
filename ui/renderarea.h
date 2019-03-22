#pragma once

#include <QWidget>
#include <QSharedPointer>

template <typename TFlatland>
class RenderArea : public QWidget
{
public:
    explicit RenderArea(QWidget* parent, const QPoint& topLeft, const QSize size, int scale,
                        QSharedPointer<TFlatland> flatland);

    QSize sizeHint() const override;

    void updateTopLeft(int x, int y);
    void updateScale(bool increment);

private:
    QSize m_size;
    QPoint m_relativeTopLeftPoint;
    int m_scale;
    QSharedPointer<TFlatland> m_flatland;

    void doPaint();

protected:
    void paintEvent(QPaintEvent* event) override;

};

#include "renderarea.inl"
