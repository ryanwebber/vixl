#pragma once

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

#include <VX/Graphics/Framebuffer.h>
#include <VX/Graphics/Private/Vulkan.h>

namespace VX::Graphics::Private {
    class SwapchainImpl final {
    VX_MAKE_NONCOPYABLE(SwapchainImpl);
    VX_DEFAULT_MOVABLE(SwapchainImpl);
    private:
        vk::raii::SwapchainKHR m_swapchain;
        std::vector<std::shared_ptr<Framebuffer>> m_framebuffers;

    public:
        explicit SwapchainImpl(vk::raii::SwapchainKHR swapchain, std::vector<std::shared_ptr<Framebuffer>> framebuffers)
            : m_swapchain(std::move(swapchain))
            , m_framebuffers(std::move(framebuffers))
        {}

        const std::vector<std::shared_ptr<Framebuffer>> &framebuffers() const {
            return m_framebuffers;
        }
    };
}
