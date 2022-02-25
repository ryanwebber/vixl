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

    public:
        explicit FramebufferImpl(vk::raii::Framebuffer framebuffer)
                : m_framebuffer(std::move(framebuffer))
        {}

        vk::raii::Framebuffer &framebuffer() { return m_framebuffer; }
        [[nodiscard]] const vk::raii::Framebuffer &framebuffer() const { return m_framebuffer; }
    };
}
