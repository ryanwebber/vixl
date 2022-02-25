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
        std::shared_ptr<CommandBuffer> m_command_buffer;

    public:
        RenderTarget(std::shared_ptr<Framebuffer> framebuffer, std::shared_ptr<CommandBuffer> command_buffer)
            : m_framebuffer(std::move(framebuffer))
            , m_command_buffer(std::move(command_buffer))
        {}

        ~RenderTarget() = default;

        [[nodiscard]] std::shared_ptr<Framebuffer> target_framebuffer() const {
            return m_framebuffer;
        }

        [[nodiscard]] std::shared_ptr<CommandBuffer> allocated_command_buffer() const {
            return m_command_buffer;
        }
    };
}
