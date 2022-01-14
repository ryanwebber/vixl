#pragma once

#include <Common/Copyable.h>
#include <Common/Noncopyable.h>

#include <Core/Closable.h>
#include <Core/EventLoop.h>

namespace Core {
    class TaskHandle final {
        VX_DEFAULT_MOVABLE(TaskHandle);
        VX_MAKE_NONCOPYABLE(TaskHandle);
    private:
        std::unique_ptr<Closable> m_Handle;
        std::vector<std::unique_ptr<Closable>> m_Connections;
    public:
        TaskHandle(std::unique_ptr<Closable> handle, std::vector<std::unique_ptr<Closable>> connections)
            : m_Handle(std::move(handle))
            , m_Connections(std::move(connections))
            {
            }

        ~TaskHandle() = default;

        void Cancel() noexcept {
            m_Handle.reset();
            m_Connections.clear();
        }
    };

    template <typename T>
    class TaskFuture {
        VX_DEFAULT_MOVABLE(TaskFuture);
        VX_MAKE_NONCOPYABLE(TaskFuture);
    private:
        std::vector<std::unique_ptr<Closable>> m_Connections;

    protected:
        std::shared_ptr<EventLoop> m_EventLoop;
        virtual std::unique_ptr<Closable> Dispatch() = 0;
        virtual std::unique_ptr<Closable> AttachListener(std::function<void(T)> listener) = 0;

    public:
        explicit TaskFuture(std::shared_ptr<EventLoop> event_loop)
            : m_EventLoop(std::move(event_loop))
            {
            }

        ~TaskFuture() = default;

        TaskFuture<T>& Bind(std::function<void(T)> callback) {
            auto connection = AttachListener(std::move(callback));
            m_Connections.push_back(std::move(connection));
            return *this;
        }

        TaskHandle Execute() {
            auto task = Dispatch();
            TaskHandle handle(std::move(task), std::move(m_Connections));
            return std::move(handle);
        }
    };
}
