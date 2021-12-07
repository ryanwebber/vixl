#pragma once

#include <chrono>
#include <functional>
#include <memory>

#include <uvw/timer.h>
#include <App/Task.h>
#include <App/EventScope.h>

class TimerLoopTask final: public Task {
public:

    using Millis = uvw::TimerHandle::Time;
    using Scope = EventScope<uvw::TimerHandle, uvw::TimerEvent>;

    TimerLoopTask(const std::string_view &name, const EventLoop&, Millis duration, bool wait_for_first_tick = true);
    ~TimerLoopTask() override = default;

    void Clear() override;
    void Close() override;

    void Stop();
    void Start();

    Scope OnTimeout(const std::function<void(void)>& listener);

private:
    std::shared_ptr<uvw::TimerHandle> m_Handle;
    Millis m_Duration;
    bool m_WaitForFirstTick;
};
