#include "window.h"

#include "lib/SimpleFlatland.h"
#include "lib/AdvancedFlatland.h"

#include <QScreen>
#include <QApplication>
#include <QGuiApplication>
#include <QSharedPointer>
#include <QPointer>

#include <memory>
#include <iostream>

using namespace flatland;
using namespace flatland::lib;

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "Use: ui <Width | 0> <Height | 0> [MaxAge] [MaxReproductivityAge]";
        return -1;
    }

    const unsigned long width = static_cast<unsigned long>(atol(argv[1]));
    const unsigned long height = static_cast<unsigned long>(atol(argv[2]));
    const unsigned long maxAge = argc < 5 ? 0 : static_cast<unsigned long>(atol(argv[3]));
    const unsigned long maxReproductivityAge = argc < 5 ? 0 : static_cast<unsigned long>(atol(argv[4]));

    QApplication a(argc, argv);
    const auto fullScreen = width == 0 || height == 0;
    const auto widthToUse = fullScreen ? static_cast<size_t>(QGuiApplication::screens().front()->size().width()) : width;
    const auto heightToUse = fullScreen ? static_cast<size_t>(QGuiApplication::screens().front()->size().height()) : height;

    std::unique_ptr<QWidget> p;
    if (maxAge == 0 && maxReproductivityAge == 0)
    {
        QSharedPointer<SimpleFlatland> f(new SimpleFlatland(CreateSimpleMap(RandomDistributionWithoutLimits{widthToUse, heightToUse})));
        p.reset(new Window(f));
    }
    else if (maxAge != 0 && maxReproductivityAge != 0)
    {
        QSharedPointer<AdvancedFlatland> f(new AdvancedFlatland(CreateAdvancedMap(RandomDistributionWithoutLimits{widthToUse, heightToUse}),
            maxAge, maxReproductivityAge));
        p.reset(new Window(f));
    }
    else
        return -1;

    if (fullScreen)
        p->setWindowState(Qt::WindowFullScreen);

    p->show();

    return a.exec();
}
