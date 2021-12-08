#pragma once

#include <memory>
#include <glm/glm.hpp>

#include <Common/Noncopyable.h>
#include <App/EventLoop.h>
#include <App/CallbackTask.h>
#include <App/TimerLoopTask.h>
#include <App/Workspace.h>

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

    std::shared_ptr<CallbackTask<std::shared_ptr<Workspace>>> m_WorkspaceOpenedCallback;
    std::shared_ptr<CallbackTask<std::shared_ptr<Workspace>>> m_WorkspaceClosedCallback;

public:
    static Dispatcher& Main();

    ~Dispatcher() = default;
    void Terminate();
    EventLoop& GetEventLoop();

    // Timers
    [[nodiscard]] TimerLoopTask& GetUILoopHandle() const { return *m_UILoop; }

    // Events
    [[nodiscard]] CallbackTask<glm::vec2>& GetWindowResizeHandle() const { return *m_WindowResizeCallback; }

    [[nodiscard]] CallbackTask<std::shared_ptr<Workspace>>& GetWorkspaceOpenedHandle() const { return *m_WorkspaceOpenedCallback; }
    [[nodiscard]] CallbackTask<std::shared_ptr<Workspace>>& GetWorkspaceClosedHandle() const { return *m_WorkspaceClosedCallback; }

};
