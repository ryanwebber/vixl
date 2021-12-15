#pragma once

#include <memory>
#include <glm/glm.hpp>

#include <Core/CallbackTask.h>
#include <Core/EventLoop.h>
#include <Core/Size.h>
#include <Core/TimerLoopTask.h>
#include <Common/Noncopyable.h>

namespace Core {
    class Dispatcher final {
    VX_MAKE_NONMOVABLE(Dispatcher);

    VX_MAKE_NONCOPYABLE(Dispatcher);

    private:
        Dispatcher();

        std::shared_ptr<EventLoop> m_EventLoop;

        // Timers
        std::shared_ptr<TimerLoopTask> m_UILoop;

        // Events
        std::shared_ptr<CallbackTask<SizeInt>> m_WindowResizeCallback;

    public:
        static Dispatcher &Main();

        ~Dispatcher() = default;

        void Terminate();

        EventLoop &GetEventLoop();

        // Timers
        [[nodiscard]] TimerLoopTask &GetUILoopHandle() const { return *m_UILoop; }

        // Events
        [[nodiscard]] CallbackTask<SizeInt> &GetWindowResizeHandle() const { return *m_WindowResizeCallback; }
    };
}