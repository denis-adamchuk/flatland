#include "window.h"

#include "renderarea.h"

#include <QApplication>
#include <QDesktopWidget>

Window::Window(QWidget *parent)
    : QWidget(parent)
    , m_renderArea(std::make_unique<RenderArea>(this, QApplication::desktop()->screenGeometry()))
{
    setWindowTitle("Flatland for QT");
    setWindowState(Qt::WindowFullScreen);
}

Window::~Window()
{

}
