#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <tuple>

#include <uvw/async.h>

#include <Core/Task.h>
#include <Core/EventScope.h>

namespace Core {

    template<typename... Args>
    class CallbackTask final : public Task {
    public:

        using Scope = EventScope<uvw::AsyncHandle, uvw::AsyncEvent>;
        using Callback = std::function<void(Args...)>;

        CallbackTask() = delete;

        CallbackTask(const std::string_view &name, const EventLoop &);

        ~CallbackTask() override = default;

        void Clear() override;

        void Close() override;

        void Notify(Args... args) const;

        [[nodiscard]] Scope OnCallback(std::function<void(Args...)> callback);

    private:
        std::shared_ptr<uvw::AsyncHandle> m_Handle;
    };

    template<typename... Args>
    CallbackTask<Args...>::CallbackTask(const std::string_view &name, const EventLoop &event)
            : Task(name, event), m_Handle(event.GetInternal()->resource<uvw::AsyncHandle>()) {
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
    void CallbackTask<Args...>::Notify(Args... args) const {
        auto send_args = std::make_shared<std::tuple<Args...>>(std::forward<Args...>(args...));
        m_Handle->data(send_args);
        m_Handle->send();
    }

    template<typename... Args>
    typename CallbackTask<Args...>::Scope CallbackTask<Args...>::OnCallback(std::function<void(Args...)> callback) {
        auto connection = m_Handle->on<uvw::AsyncEvent>(
                [cb = std::move(callback)](const uvw::AsyncEvent &ev, const uvw::AsyncHandle &handle) {
                    auto args = handle.data<std::tuple<Args...>>();
                    std::apply(cb, *args);
                });

        return std::move(Scope(m_Handle, connection));
    }
}
