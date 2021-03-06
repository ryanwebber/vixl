#pragma once

#include <VX/Core/Size.h>
#include <VX/Core/Window.h>

namespace VX::Core {
    class RenderTarget final {
    private:
        SizeInt m_viewport_size;
    public:
        explicit RenderTarget(SizeInt size) : m_viewport_size(size) {};
        ~RenderTarget() = default;

        [[nodiscard]] SizeInt viewport_size() const { return m_viewport_size; }

        static RenderTarget to_window(const Window& window) {
            return RenderTarget(window.native_window().size());
        }
    };
}
