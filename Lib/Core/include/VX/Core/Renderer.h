#pragma once

#include <VX/Noncopyable.h>

#include <VX/Core/RenderStack.h>

namespace VX::Core {
    class Renderer final {
        VX_MAKE_NONCOPYABLE(Renderer);
        VX_MAKE_NONMOVABLE(Renderer);

    private:
        RenderStack m_RenderStack;

    public:
        Renderer() = default;
        ~Renderer() = default;

        void RenderFrame();
        void Destroy();

        RenderStack& GetRenderStack() { return m_RenderStack; }
    };
}
