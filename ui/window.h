#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

#include <memory>

class RenderArea;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(QWidget *parent = 0);
    ~Window();

private:
    std::unique_ptr<RenderArea> m_renderArea;
};

#endif // WINDOW_H
