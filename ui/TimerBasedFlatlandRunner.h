#pragma once

#include <QTimer>
#include <QPointer>
#include <QWeakPointer>
#include <QSharedPointer>

#include <functional>

class QTimer;

namespace flatland
{
namespace lib
{
    struct IFlatland;
}
}

class TimerBasedFlatlandRunner
{
public:
    TimerBasedFlatlandRunner(QSharedPointer<flatland::lib::IFlatland> flatland,
                             std::function<void()> onNextGeneration,
                             std::function<void()> onEnd);

    void AdjustInterval(int delta);

private:
    QSharedPointer<flatland::lib::IFlatland> m_flatland;
    QPointer<QTimer> m_timer;

    std::function<void()> m_onNextGeneration;
    std::function<void()> m_onEnd;
};
