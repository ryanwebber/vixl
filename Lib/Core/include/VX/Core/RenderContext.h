#pragma once

#include <VX/Noncopyable.h>
#include <VX/Core/RenderTarget.h>

namespace VX::Core {
    class RenderContext final {
        VX_MAKE_NONCOPYABLE(RenderContext);
        VX_MAKE_NONMOVABLE(RenderContext);
    private:
        RenderTarget m_render_target;
    public:
        explicit RenderContext(const RenderTarget &render_target)
            : m_render_target(render_target)
        {};

        ~RenderContext() = default;

        [[nodiscard]] const RenderTarget &render_target() const { return m_render_target; }
    };
}
