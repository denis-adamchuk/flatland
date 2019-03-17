#include "window.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "Use: ui <MaxAge> <MaxReproductivityAge>";
        return -1;
    }

    const unsigned long maxAge = static_cast<unsigned long>(atol(argv[1]));
    const unsigned long maxReproductivityAge = static_cast<unsigned long>(atol(argv[2]));
    if (maxAge == 0 || maxReproductivityAge == 0)
    {
        std::cout << "Invalid argument";
        return -1;
    }

    QApplication a(argc, argv);
    Window w(nullptr, maxAge, maxReproductivityAge);
    w.show();

    return a.exec();
}
