
#include <uvw/timer.h>
#include <App/TimerTask.h>

TimerTask::TimerTask(const std::string_view &name, const EventLoop& event_loop, Millis duration)
    : Task(name, event_loop)
    , m_Duration(duration)
    , m_Handle(event_loop.GetInternal()->resource<uvw::TimerHandle>())
{
}

void TimerTask::Start() {
    m_Handle->start(m_Duration, { });
}

void TimerTask::Stop() {
    m_Handle->stop();
}

void TimerTask::Clear() {
    m_Handle->clear();
}

void TimerTask::Close() {
    m_Handle->close();
}

TimerTask::Scope TimerTask::OnTimeout(const std::function<void(void)> &listener) {
    auto connection = m_Handle->on<uvw::TimerEvent>([&](const uvw::TimerEvent &ev, const uvw::TimerHandle &handle) {
        listener();
    });

    return std::move(Scope(m_Handle, connection));
}
