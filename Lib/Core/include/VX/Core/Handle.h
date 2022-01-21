#pragma once

#include <memory>
#include <bgfx/bgfx.h>

namespace VX::Core {
    template<typename T>
    class Handle final {
    private:
        T m_handle;
    public:
        explicit Handle(T handle)
            : m_handle(handle)
        {
        }

        // Move
        Handle(Handle&&) noexcept = default;
        Handle& operator=(Handle&&) noexcept = default;

        // Copy
        Handle(const Handle&) noexcept = default;
        Handle& operator=(const Handle&) noexcept = default;

        ~Handle() {
            bgfx::destroy(m_handle);
        }

        T get() const { return m_handle; };

        T operator*() const { return m_handle; }

        [[nodiscard]] bool is_valid() const { return m_handle.idx > 0; }
    };
}
