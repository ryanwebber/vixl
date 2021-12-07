#pragma once

#include <memory>

#include <uvw/emitter.h>
#include <Common/Noncopyable.h>

#include <App/Logger.h>

/**
 * Manages a connection to an event by removing the corresponding event listener
 * when this object gets destructed.
 * @tparam T The internal handle type
 * @tparam E The internal event type
 */
template<typename T, typename E>
class EventScope final {
    VX_MAKE_NONCOPYABLE(EventScope);

public:
    typedef T Handler;
    typedef typename T::template Connection<E> Connection;

    EventScope();
    EventScope(std::shared_ptr<Handler>, Connection);
    ~EventScope();

    EventScope(EventScope&&) noexcept ;
    EventScope& operator=(EventScope&&) noexcept ;

    void Close();

private:
    std::shared_ptr<Handler> m_Handler;
    Connection m_Connection;
};

template<typename T, typename E>
EventScope<T, E>::EventScope() = default;


template<typename T, typename E>
EventScope<T, E>::EventScope(std::shared_ptr<Handler> handler, Connection connection)
    : m_Handler(std::move(handler))
    , m_Connection(std::move(connection))
{
}

template<typename T, typename E>
EventScope<T, E>::EventScope(EventScope &&other) noexcept {
    m_Handler = std::move(other.m_Handler);
    m_Connection = std::move(other.m_Connection);
}

template<typename T, typename E>
EventScope<T, E>::~EventScope() {
    Close();
}

template<typename T, typename E>
EventScope<T, E> &EventScope<T, E>::operator=(EventScope &&other) noexcept {
    Close();
    m_Handler = std::move(other.m_Handler);
    m_Connection = std::move(other.m_Connection);
    return *this;
}

template<typename T, typename E>
void EventScope<T, E>::Close() {
    if (m_Handler != nullptr) {
        m_Handler->erase(m_Connection);
    }
}
