#pragma once

#include <memory>

#include <VX/Noncopyable.h>

#include <VX/Core/RenderStack.h>
#include <VX/Graphics/Instance.h>
#include <VX/Platform/NativeWindow.h>

namespace VX::Core {

    using namespace VX::Platform;

    class Renderer final {
        VX_MAKE_NONCOPYABLE(Renderer);
        VX_MAKE_NONMOVABLE(Renderer);

    private:
        RenderStack m_render_stack;
        std::shared_ptr<Graphics::Instance> m_graphics;

    public:
        explicit Renderer(std::shared_ptr<Graphics::Instance> graphics)
            : m_graphics(std::move(graphics))
        {};

        ~Renderer() = default;

        void render_frame();
        void destroy();

        RenderStack& render_stack() { return m_render_stack; }
        [[nodiscard]] const RenderStack& render_stack() const { return m_render_stack; }

        std::shared_ptr<Graphics::Instance> &graphics() { return m_graphics; }
        [[nodiscard]] const std::shared_ptr<Graphics::Instance> &graphics() const { return m_graphics; }
    };
}
