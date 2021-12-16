#pragma once

#include <chrono>
#include <functional>
#include <memory>

#include <uvw/timer.h>
#include <Core/Task.h>
#include <Core/EventScope.h>

namespace Core {
    class TimerTask final : public Task {
    public:

        using Millis = uvw::TimerHandle::Time;
        using Scope = EventScope<uvw::TimerHandle, uvw::TimerEvent>;

        TimerTask(const std::string_view &name, const EventLoop &, Millis duration);

        ~TimerTask() override = default;

        void Clear() override;

        void Close() override;

        void Stop();

        void Start();

        [[nodiscard]] Scope OnTimeout(const std::function<void(void)> &listener);

    private:
        std::shared_ptr<uvw::TimerHandle> m_Handle;
        Millis m_Duration;
    };
}