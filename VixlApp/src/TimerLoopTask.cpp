
#include <uvw/timer.h>
#include <App/TimerLoopTask.h>

TimerLoopTask::TimerLoopTask(const std::string_view &name, const EventLoop& event_loop, Millis duration, bool start_now)
    : Task(name, event_loop)
    , m_Duration(duration)
    , m_WaitForFirstTick(start_now)
    , m_Handle(event_loop.GetInternal()->resource<uvw::TimerHandle>())
{
}

void TimerLoopTask::Start() {
    if (m_WaitForFirstTick)
        m_Handle->start({ }, m_Duration);
    else
        m_Handle->start(m_Duration, m_Duration);
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

TimerLoopTask::Scope TimerLoopTask::OnTimeout(const std::function<void(void)> &listener) {
    auto connection = m_Handle->on<uvw::TimerEvent>([&](const uvw::TimerEvent &ev, const uvw::TimerHandle &handle) {
        listener();
    });

    return std::move(Scope(m_Handle, connection));
}
