#include "window.h"

#include <QScreen>
#include <QApplication>
#include <QGuiApplication>

#include <iostream>

int main(int argc, char *argv[])
{
    if (argc < 5)
    {
        std::cout << "Use: ui <Width> <Height> <MaxAge> <MaxReproductivityAge>";
        return -1;
    }

    const unsigned long width = static_cast<unsigned long>(atol(argv[1]));
    const unsigned long height = static_cast<unsigned long>(atol(argv[2]));
    const unsigned long maxAge = static_cast<unsigned long>(atol(argv[3]));
    const unsigned long maxReproductivityAge = static_cast<unsigned long>(atol(argv[4]));
    if (maxAge == 0 || maxReproductivityAge == 0)
    {
        std::cout << "Invalid argument";
        return -1;
    }

    QApplication a(argc, argv);
    Window w(nullptr,
             width == 0 ? static_cast<size_t>(QGuiApplication::screens().front()->size().width()) : width,
             height == 0 ? static_cast<size_t>(QGuiApplication::screens().front()->size().height()) : height,
             maxAge, maxReproductivityAge);
    w.show();

    return a.exec();
}
