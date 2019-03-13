#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>

#include "lib/SimpleFlatland.h"

class RenderArea : public QWidget
{
    Q_OBJECT
public:
    explicit RenderArea(QWidget *parent, const QRect& rc);

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    flatland::lib::SimpleFlatland m_flatland;

signals:

public slots:
    void plotNextGeneration();
};

#endif // RENDERAREA_H
