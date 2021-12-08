#pragma once

#include <chrono>
#include <functional>
#include <memory>

#include <uvw/async.h>

#include <App/Assert.h>
#include <App/Task.h>
#include <App/EventScope.h>

template<typename... Args>
class CallbackTask final: public Task {
public:

    using Scope = EventScope<uvw::AsyncHandle, uvw::AsyncEvent>;

    CallbackTask() = delete;
    CallbackTask(const std::string_view &name, const EventLoop&);
    ~CallbackTask() override = default;

    void Clear() override;
    void Close() override;

    void Notify(Args... args);

    [[nodiscard]] Scope OnCallback(std::function<void(Args...)> listener);

private:
    std::shared_ptr<uvw::AsyncHandle> m_Handle;
};

template<typename... Args>
CallbackTask<Args...>::CallbackTask(const std::string_view &name, const EventLoop &event)
    : Task(name, event)
    , m_Handle(event.GetInternal()->resource<uvw::AsyncHandle>())
{
}

template<typename... Args>
void CallbackTask<Args...>::Clear() {
    m_Handle->clear();
}

template<typename... Args>
void CallbackTask<Args...>::Close() {
    m_Handle->close();
}

template<typename... Args>
void CallbackTask<Args...>::Notify(Args... args) {
    auto send_args = std::make_shared<std::tuple<Args...>>(args...);
    m_Handle->data(send_args);
    m_Handle->send();
}

template<typename... Args>
typename CallbackTask<Args...>::Scope CallbackTask<Args...>::OnCallback(std::function<void(Args...)> listener) {
    ASSERT(m_Handle != nullptr, "Callback handle was not initialized");
    auto connection = m_Handle->on<uvw::AsyncEvent>([&](uvw::AsyncEvent &ev, const uvw::AsyncHandle &handle) {
        auto args = handle.data<std::tuple<Args...>>();
        std::apply(listener, *args);
    });

    return std::move(Scope(m_Handle, connection));
}
