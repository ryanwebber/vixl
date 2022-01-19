#pragma once

#include <VX/Noncopyable.h>

#include <VX/Core/RenderStack.h>

namespace VX::Core {
    class Renderer final {
        VX_MAKE_NONCOPYABLE(Renderer);
        VX_MAKE_NONMOVABLE(Renderer);

    private:
        RenderStack m_render_stack;

    public:
        Renderer() = default;
        ~Renderer() = default;

        void render_frame();
        void destroy();

        RenderStack& render_stack() { return m_render_stack; }
    };
}
