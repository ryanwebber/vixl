#pragma once

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

#include <VX/Graphics/Graphics.h>
#include <VX/Graphics/Vulkan.h>

namespace VX::Graphics {
    class Swapchain final {
        VX_MAKE_NONCOPYABLE(Swapchain);
        VX_DEFAULT_MOVABLE(Swapchain);
    private:
        vk::raii::SwapchainKHR m_swapchain;
        vk::PresentModeKHR present_mode;
        vk::SurfaceFormatKHR surface_format;
        vk::SurfaceCapabilitiesKHR surface_capabilities;

        std::vector<std::shared_ptr<RenderTargetHandle>> m_render_targets;
        std::vector<std::shared_ptr<RenderContextHandle>> m_render_contexts;
        std::vector<std::shared_ptr<CommandBufferHandle>> m_command_buffers;
    public:
        ~Swapchain() = default;
    };
}
