#include "window-android.h"

#include "lib/SimpleFlatland.h"
#include "lib/AdvancedFlatland.h"

#include <QScreen>
#include <QApplication>
#include <QGuiApplication>

#include <memory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    const unsigned long defaultScale = 8;
    const unsigned long maxAge = 100;
    const unsigned long maxReproductivityAge = 75;

    // Android application is full-screen, but flatland dimensions are reduced with a default scale (unlike in Desktop)
    const auto widthToUse = static_cast<size_t>(QGuiApplication::screens().front()->size().width()) / defaultScale;
    const auto heightToUse = static_cast<size_t>(QGuiApplication::screens().front()->size().height()) / defaultScale;

    QSharedPointer<SimpleFlatland> f1(
        new SimpleFlatland(CreateSimpleMap(RandomDistributionWithoutLimits{widthToUse, heightToUse})));
    QSharedPointer<AdvancedFlatland> f2(
        new AdvancedFlatland(CreateAdvancedMap(RandomDistributionWithoutLimits{widthToUse, heightToUse}),
            maxAge, maxReproductivityAge));

    std::unique_ptr<QWidget> p(new Window(f1, f2, defaultScale));

    p->setWindowState(Qt::WindowFullScreen);

    p->show();

    return a.exec();
}
