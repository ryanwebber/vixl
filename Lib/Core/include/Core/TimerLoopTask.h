#pragma once

#include <chrono>
#include <functional>
#include <memory>

#include <uvw/timer.h>
#include <Core/Task.h>
#include <Core/EventScope.h>

namespace Core {
    class TimerLoopTask final : public Task {
    public:

        using Millis = uvw::TimerHandle::Time;
        using Scope = EventScope<uvw::TimerHandle, uvw::TimerEvent>;

        TimerLoopTask(const std::string_view &name, const EventLoop &, Millis duration);

        ~TimerLoopTask() override = default;

        void Clear() override;

        void Close() override;

        void Stop();

        void Start();

        Scope OnTimeout(const std::function<void(void)> &listener);

    private:
        std::shared_ptr<uvw::TimerHandle> m_Handle;
        Millis m_Duration;
    };
}