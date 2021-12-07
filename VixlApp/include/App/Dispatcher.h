#pragma once

#include <memory>
#include <glm/glm.hpp>

#include <Common/Noncopyable.h>
#include <App/EventLoop.h>
#include <App/CallbackTask.h>
#include "TimerLoopTask.h"

class Dispatcher final {
    VX_MAKE_NONMOVABLE(Dispatcher);
    VX_MAKE_NONCOPYABLE(Dispatcher);

private:
    Dispatcher();
    std::shared_ptr<EventLoop> m_EventLoop;

    // Timers
    std::shared_ptr<TimerLoopTask> m_UILoop;

    // Events
    std::shared_ptr<CallbackTask<glm::vec2>> m_WindowResizeCallback;

public:
    static Dispatcher& Main();

    ~Dispatcher() = default;
    void Terminate();
    EventLoop& GetEventLoop();

    // Timers
    [[nodiscard]] TimerLoopTask& GetUILoopHandle() const { return *m_UILoop; }

    // Events
    [[nodiscard]] CallbackTask<glm::vec2>& GetWindowResizeHandle() const { return *m_WindowResizeCallback; }
};
