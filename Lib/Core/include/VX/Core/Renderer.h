#pragma once

#include <memory>

#include <VX/Noncopyable.h>

#include <VX/Graphics/Graphics.h>

#include <VX/Core/RenderStack.h>
#include <VX/Platform/NativeWindow.h>

namespace VX::Core {

    using namespace VX::Platform;

    class Renderer final {
        VX_MAKE_NONCOPYABLE(Renderer);
        VX_MAKE_NONMOVABLE(Renderer);

    private:
        RenderStack m_render_stack;
        Graphics::Instance m_graphics;

    public:
        explicit Renderer(Graphics::Instance graphics)
            : m_graphics(std::move(graphics))
        {}

        ~Renderer() = default;

        void render_frame();
        void destroy();

        RenderStack& render_stack() { return m_render_stack; }
        [[nodiscard]] const RenderStack& render_stack() const { return m_render_stack; }

        Graphics::Instance &graphics() { return m_graphics; }
        [[nodiscard]] const Graphics::Instance &graphics() const { return m_graphics; }
    };
}
