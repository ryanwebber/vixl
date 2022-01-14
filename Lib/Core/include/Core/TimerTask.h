#pragma once

#include <chrono>
#include <functional>
#include <memory>

#include <uvw/timer.h>
#include <Core/Closable.h>
#include <Core/Task.h>

namespace Core {
    class TimerTask final : public Task {
    public:
        using Millis = uvw::TimerHandle::Time;

        TimerTask(const std::string_view &name, const EventLoop &, Millis duration);
        ~TimerTask() override = default;

        void Clear() override;
        void Close() override;
        void Stop();
        void Start();

        [[nodiscard]] Closable OnTimeout(const std::function<void(void)> &listener);

    private:
        std::shared_ptr<uvw::TimerHandle> m_Handle;
        Millis m_Duration;
    };
}
