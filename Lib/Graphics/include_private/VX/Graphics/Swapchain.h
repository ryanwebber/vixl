#pragma once

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

#include <VX/Graphics/Graphics.h>
#include <VX/Graphics/Vulkan.h>

namespace VX::Graphics {

    class SwapFrame {
        VX_MAKE_NONCOPYABLE(SwapFrame);
        VX_DEFAULT_MOVABLE(SwapFrame);
    private:
        RenderTargetHandle m_target_handle;
        RenderContextHandle m_context_handle;
    public:
        SwapFrame(RenderTargetHandle target_handle, RenderContextHandle context_handle)
            : m_target_handle(std::move(target_handle))
            , m_context_handle(std::move(context_handle))
        {}

        ~SwapFrame() = default;
    };

    class SwapBuffer {
        VX_MAKE_NONCOPYABLE(SwapBuffer);
        VX_DEFAULT_MOVABLE(SwapBuffer);
    private:
        CommandBufferHandle m_command_buffer_handle;
    public:
        explicit SwapBuffer(CommandBufferHandle command_buffer_handle)
            : m_command_buffer_handle(std::move(command_buffer_handle))
        {}

        ~SwapBuffer() = default;
    };

    class Swapchain final {
        VX_MAKE_NONCOPYABLE(Swapchain);
        VX_DEFAULT_MOVABLE(Swapchain);
    private:
        uint32_t m_frame_index { 0 };
        vk::raii::SwapchainKHR m_swapchain;
        vk::PresentModeKHR m_present_mode;
        vk::SurfaceFormatKHR m_surface_format;
        vk::SurfaceCapabilitiesKHR m_surface_capabilities;
        std::vector<SwapFrame> m_swap_frame_pool;
        std::vector<SwapBuffer> m_swap_buffer_pool;

    public:
        Swapchain(vk::raii::SwapchainKHR swapchain,
                  vk::PresentModeKHR present_mode,
                  vk::SurfaceFormatKHR surface_format,
                  vk::SurfaceCapabilitiesKHR surface_capabilities,
                  std::vector<SwapFrame> swap_frame_pool,
                  std::vector<SwapBuffer> swap_buffer_pool)
            : m_swapchain(std::move(swapchain))
            , m_present_mode(present_mode)
            , m_surface_format(surface_format)
            , m_surface_capabilities(surface_capabilities)
            , m_swap_frame_pool(std::move(swap_frame_pool))
            , m_swap_buffer_pool(std::move(swap_buffer_pool))
        {};

        ~Swapchain() = default;

        std::optional<SwapState> try_acquire_next_swap_state() {
            return { };
        }
    };
}
