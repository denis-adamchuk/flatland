#ifndef WINDOW_H
#define WINDOW_H

#include "lib/SimpleFlatland.h"

#include <QWidget>
#include <QSharedPointer>

class RenderArea;
class QTimer;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(QWidget *parent = 0);

public slots:
    void plotNextGeneration();

protected:
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    QSharedPointer<flatland::lib::SimpleFlatland> m_flatland;
    RenderArea* m_renderArea;
    QTimer* m_timer;
};

#endif // WINDOW_H
