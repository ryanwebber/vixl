#pragma once

#include <memory>
#include <bgfx/bgfx.h>

namespace VX::Core {
    template<typename T>
    class Handle final {
    private:
        T m_Handle;
    public:
        explicit Handle(T handle)
            : m_Handle(handle)
            {
            }

        // Move
        Handle(Handle&&) noexcept = default;
        Handle& operator=(Handle&&) noexcept = default;

        // Copy
        Handle(const Handle&) noexcept = default;
        Handle& operator=(const Handle&) noexcept = default;

        ~Handle() {
            bgfx::destroy(m_Handle);
        }

        T Get() const { return m_Handle; };

        T operator*() const { return m_Handle; }
    };
}
