#pragma once

#include <uvw/loop.h>

class Task;

class EventLoop final {
private:
    std::shared_ptr<uvw::Loop> m_Loop;

public:
    EventLoop()
        : m_Loop(uvw::Loop::create())
    { }

    ~EventLoop() = default;

    [[nodiscard]] std::shared_ptr<uvw::Loop> GetInternal() const { return m_Loop; }
    void Run() { m_Loop->run(); }

    template<typename R, typename... Args>
    std::shared_ptr<R> Register(const std::string_view &name, Args&&... args) {
        static_assert(std::is_base_of<Task, R>::value, "Not an EventHandle type");
        return std::make_shared<R>(name, *this, args...);
    }
};
