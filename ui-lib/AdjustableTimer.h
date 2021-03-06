#pragma once

#include <functional>

class QTimer;
class QWidget;

// TODO - Bad design, too specific thing for a class
// QTimer-wrapper that calls a callback when period is expired
class AdjustableTimer
{
public:
    AdjustableTimer(QWidget* parent, std::function<void()> onTimer);

    void AdjustInterval(int delta);
    int GetInterval() const;

private:
    QTimer* m_timer;

    std::function<void()> m_onTimer;
};
