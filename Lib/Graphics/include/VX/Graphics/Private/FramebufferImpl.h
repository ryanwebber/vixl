#pragma once

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

#include <VX/Graphics/CommandBuffer.h>
#include <VX/Graphics/Private/Vulkan.h>

namespace VX::Graphics::Private {
    class FramebufferImpl final {
        VX_MAKE_NONCOPYABLE(FramebufferImpl);
        VX_DEFAULT_MOVABLE(FramebufferImpl);
    private:
        vk::raii::Framebuffer m_framebuffer;
        std::shared_ptr<CommandBuffer> m_associated_command_buffer;

    public:
        explicit FramebufferImpl(vk::raii::Framebuffer framebuffer, std::shared_ptr<CommandBuffer> associated_command_buffer)
                : m_framebuffer(std::move(framebuffer))
                , m_associated_command_buffer(std::move(associated_command_buffer))
        {}

        [[nodiscard]] std::shared_ptr<CommandBuffer> associated_command_buffer() const { return m_associated_command_buffer; }
    };
}
