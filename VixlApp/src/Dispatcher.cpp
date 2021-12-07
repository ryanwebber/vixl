#include <App/Dispatcher.h>
#include <App/TimerTask.h>

#ifndef TARGET_FPS
  #define TARGET_FPS 60
#endif

std::unique_ptr<Dispatcher> g_Instance = nullptr;

Dispatcher::Dispatcher()
    : m_EventLoop(std::make_shared<EventLoop>())
{
    // Initialize the event loop
    static_assert(TARGET_FPS > 0, "Invalid target FPS");
    auto mills_per_frame = TimerTask::Millis(1000 / TARGET_FPS);
    m_UILoop = m_EventLoop->Register<TimerLoopTask>("UILoop", mills_per_frame);
    m_UILoop->Start();

    // Initialize the callbacks
    m_WindowResizeCallback = m_EventLoop->Register<CallbackTask<glm::vec2>>("OnWindowResize");
}

EventLoop &Dispatcher::GetEventLoop() {
    return *m_EventLoop;
}

Dispatcher &Dispatcher::Main() {
    if (g_Instance == nullptr) {
        auto instance = new Dispatcher();
        g_Instance = std::unique_ptr<Dispatcher>(instance);
    }

    return *g_Instance;
}

void Dispatcher::Terminate() {
    m_EventLoop->Close();
}