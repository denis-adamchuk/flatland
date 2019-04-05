#include "AdjustableTimer.h"

#include <QTimer>
#include <QWidget>

namespace
{

const int sc_minTimerInterval = 1;
const int sc_maxTimerInterval = 2000;
const int sc_defaultTimerInterval = 200;

}

AdjustableTimer::AdjustableTimer(QWidget* parent, std::function<void()> onTimer)
    : m_timer(new QTimer(parent))
    , m_onTimer(onTimer)
{
    m_timer->setTimerType(Qt::TimerType::PreciseTimer);
    m_timer->setInterval(sc_minTimerInterval);
    m_timer->callOnTimeout(
        [this]()
    {
        m_onTimer();
    });
    m_timer->start(sc_defaultTimerInterval);
}

void AdjustableTimer::AdjustInterval(int delta)
{
    const auto currentInterval = m_timer->interval();
    const auto interval = std::min(std::max(currentInterval - delta, sc_minTimerInterval), sc_maxTimerInterval);
    m_timer->setInterval(interval);
}

int AdjustableTimer::GetInterval() const
{
    return m_timer->interval();
}
