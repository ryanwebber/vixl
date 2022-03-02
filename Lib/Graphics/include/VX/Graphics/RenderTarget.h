#pragma once

#include <VX/Copyable.h>

#include <glm/glm.hpp>

#include <VX/Graphics/CommandBuffer.h>
#include <VX/Graphics/Framebuffer.h>

namespace VX::Graphics {
    class RenderTarget final {
        VX_DEFAULT_COPYABLE(RenderTarget);
        VX_DEFAULT_MOVABLE(RenderTarget);
    private:
        std::shared_ptr<Framebuffer> m_framebuffer;
        glm::vec<2, int> m_dimensions;

    public:
        explicit RenderTarget(std::shared_ptr<Framebuffer> framebuffer, glm::vec<2, int> dimensions)
            : m_framebuffer(std::move(framebuffer))
            , m_dimensions(dimensions)
        {}

        ~RenderTarget() = default;

        [[nodiscard]] std::shared_ptr<Framebuffer> target_framebuffer() const {
            return m_framebuffer;
        }

        [[nodiscard]] glm::vec<2, int> dimensions() const { return m_dimensions; }
    };
}
