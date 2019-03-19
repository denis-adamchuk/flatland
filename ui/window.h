#ifndef WINDOW_H
#define WINDOW_H

#include "lib/AdvancedFlatland.h"

#include <QWidget>
#include <QSharedPointer>

class RenderArea;
class QTimer;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(QWidget *parent, unsigned long width, unsigned long height,
           unsigned long maxAge, unsigned long maxReproductivityAge);

public slots:
    void plotNextGeneration();

protected:
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    QSharedPointer<flatland::lib::AdvancedFlatland> m_flatland;
    RenderArea* m_renderArea;
    QTimer* m_timer;
};

#endif // WINDOW_H
