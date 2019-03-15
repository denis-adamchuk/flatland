#ifndef RENDERAREA_H
#define RENDERAREA_H

#include "lib/SimpleFlatland.h"

#include <QWidget>

class RenderArea : public QWidget
{
    Q_OBJECT
public:
    explicit RenderArea(QWidget* parent, const QPoint& topLeft, const QSize size, int scale,
                        QSharedPointer<flatland::lib::SimpleFlatland> flatland);

    QSize sizeHint() const override;

    void updateTopLeft(int x, int y);
    void updateScale(bool increment);

private:
    QSize m_size;
    QPoint m_relativeTopLeftPoint;
    int m_scale;
    QSharedPointer<flatland::lib::SimpleFlatland> m_flatland;

protected:
    void paintEvent(QPaintEvent* event) override;

};

#endif // RENDERAREA_H
