#pragma once

#include <Common/Noncopyable.h>

#include <Core/RenderStack.h>

namespace Core {
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
    };
}
