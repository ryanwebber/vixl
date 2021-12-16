#pragma once

#include <memory>

namespace Core {
    template<typename T>
    class Handle final {
    private:
        T m_Handle;
    public:
        explicit Handle(T handle);

        ~Handle();

        T Get() const { return m_Handle; };

        T operator*() const { return m_Handle; }
    };

    template<typename T>
    Handle<T>::Handle(T handle)
        : m_Handle(handle)
        {
        }

    template<typename T>
    Handle<T>::~Handle() {
        bgfx::destroy(m_Handle);
    }
}
