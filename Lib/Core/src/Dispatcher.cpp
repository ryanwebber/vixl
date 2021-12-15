#include <Core/Dispatcher.h>
#include <Core/TimerTask.h>

#ifndef TARGET_FPS
  #define TARGET_FPS 60
#endif

namespace Core {
    std::unique_ptr<Dispatcher> g_Instance = nullptr;

    Dispatcher::Dispatcher()
            : m_EventLoop(std::make_shared<EventLoop>()) {
        // Initialize the event loop
        static_assert(TARGET_FPS > 0, "Invalid target FPS");
        auto mills_per_frame = TimerTask::Millis(
                500 / TARGET_FPS); // Why does this need to be scaled by 2x to work right?
        m_UILoop = m_EventLoop->Register<TimerLoopTask>("UILoop", mills_per_frame);
        m_UILoop->Start();

        // Initialize the callbacks
        m_WindowResizeCallback = m_EventLoop->Register<CallbackTask<SizeInt>>("OnWindowResize");
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
}