#pragma once

#include <VX/Copyable.h>

#include <VX/Graphics/CommandBuffer.h>
#include <VX/Graphics/Framebuffer.h>

namespace VX::Graphics {
    class RenderTarget final {
        VX_DEFAULT_COPYABLE(RenderTarget);
        VX_DEFAULT_MOVABLE(RenderTarget);
    private:
        std::shared_ptr<Framebuffer> m_framebuffer;

    public:
        explicit RenderTarget(std::shared_ptr<Framebuffer> framebuffer)
            : m_framebuffer(std::move(framebuffer))
        {}

        ~RenderTarget() = default;

        [[nodiscard]] std::shared_ptr<Framebuffer> target_framebuffer() const {
            return m_framebuffer;
        }
    };
}
