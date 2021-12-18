
#include <uvw/timer.h>
#include <Core/TimerLoopTask.h>

namespace Core {
    TimerLoopTask::TimerLoopTask(const std::string_view &name, const EventLoop &event_loop, Millis duration)
            : Task(name, event_loop), m_Duration(duration),
              m_Handle(event_loop.GetInternal()->resource<uvw::TimerHandle>()) {
    }

    void TimerLoopTask::Start() {
        m_Handle->start({}, m_Duration);
    }

    void TimerLoopTask::Stop() {
        m_Handle->stop();
    }

    void TimerLoopTask::Clear() {
        m_Handle->clear();
    }

    void TimerLoopTask::Close() {
        m_Handle->close();
    }

    Closable TimerLoopTask::OnTimeout(const std::function<void(void)> &listener) {
        auto connection = m_Handle->on<uvw::TimerEvent>([&](const uvw::TimerEvent &ev, const uvw::TimerHandle &handle) {
            listener();
        });

        Closable closable([=](){
            m_Handle->erase(connection);
        });

        return std::move(closable);
    }
}
