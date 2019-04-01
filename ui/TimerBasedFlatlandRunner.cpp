#include "TimerBasedFlatlandRunner.h"

#include "lib/FlatlandItf.h"

#include <QTimer>

namespace
{

const int sc_minTimerInterval = 1;
const int sc_maxTimerInterval = 2000;
const int sc_defaultTimerInterval = 200;

}

using namespace flatland;
using namespace flatland::lib;

TimerBasedFlatlandRunner::TimerBasedFlatlandRunner(QSharedPointer<IFlatland> flatland,
                                                   std::function<void()> onNextGeneration,
                                                   std::function<void()> onEnd)
    : m_flatland(flatland)
    , m_timer(new QTimer(nullptr))
    , m_onNextGeneration(onNextGeneration)
    , m_onEnd(onEnd)
{
    m_timer->setTimerType(Qt::TimerType::PreciseTimer);
    m_timer->setInterval(sc_minTimerInterval);
    QWeakPointer<IFlatland> wp = m_flatland;
    m_timer->callOnTimeout(
        [wp, this]()
    {
        auto sp = wp.lock();
        if (!sp)
            return;

        sp->Run();
        m_onNextGeneration();
    });
    m_timer->start(sc_defaultTimerInterval);
}

void TimerBasedFlatlandRunner::AdjustInterval(int delta)
{
    const auto currentInterval = m_timer->interval();
    const auto interval = std::min(std::max(currentInterval - delta, sc_minTimerInterval), sc_maxTimerInterval);
    m_timer->setInterval(interval);
}
