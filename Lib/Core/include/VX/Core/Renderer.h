#pragma once

#include <memory>

#include <VX/Noncopyable.h>

#include <VX/Core/NativeWindow.h>
#include <VX/Core/RenderStack.h>

namespace VX::Core {
    class Renderer final {
        VX_MAKE_NONCOPYABLE(Renderer);
        VX_MAKE_NONMOVABLE(Renderer);

    private:
        RenderStack m_render_stack;
        NativeWindow m_native_window;

    public:
        explicit Renderer(NativeWindow nw)
            : m_native_window(nw)
        {};

        ~Renderer() = default;

        void render_frame();
        void destroy();

        RenderStack& render_stack() { return m_render_stack; }
    };
}
