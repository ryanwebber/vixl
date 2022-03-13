#pragma once

#include <VX/Copyable.h>
#include <VX/Expected.h>
#include <VX/Noncopyable.h>

#include <VX/Graphics/Graphics.h>
#include <VX/Graphics/QueueSupport.h>
#include <VX/Graphics/ResourceManager.h>
#include <VX/Graphics/Vulkan.h>

namespace VX::Graphics {

    class SwapFrame final {
        VX_MAKE_NONCOPYABLE(SwapFrame);
        VX_DEFAULT_MOVABLE(SwapFrame);
    private:
        std::shared_ptr<RenderTargetHandle> m_target_handle;
    public:
        explicit SwapFrame(RenderTargetHandle target_handle)
            : m_target_handle(std::make_shared<RenderTargetHandle>(std::move(target_handle)))
        {}

        [[nodiscard]] const std::shared_ptr<RenderTargetHandle> &render_target_handle() const { return m_target_handle; }

        ~SwapFrame() = default;
    };

    class SwapContext final {
        VX_MAKE_NONCOPYABLE(SwapContext);
        VX_DEFAULT_MOVABLE(SwapContext);
    private:
        std::shared_ptr<RenderContextHandle> m_render_context_handle;
        std::shared_ptr<CommandBufferHandle> m_command_buffer_handle;
    public:
        SwapContext(RenderContextHandle render_context_handle, CommandBufferHandle command_buffer_handle)
            : m_render_context_handle(std::make_shared<RenderContextHandle>(std::move(render_context_handle)))
            , m_command_buffer_handle(std::make_shared<CommandBufferHandle>(std::move(command_buffer_handle)))
        {}

        ~SwapContext() = default;

        [[nodiscard]] const std::shared_ptr<RenderContextHandle> &render_context_handle() const { return m_render_context_handle; }
        [[nodiscard]] const std::shared_ptr<CommandBufferHandle> &command_buffer_handle() const { return m_command_buffer_handle; }
    };

    class Swapchain final {
        VX_MAKE_NONCOPYABLE(Swapchain);
        VX_DEFAULT_MOVABLE(Swapchain);
    private:
        uint32_t m_frame_index { 0 };
        vk::raii::SwapchainKHR m_swapchain;
        std::shared_ptr<vk::raii::Device> m_device;
        std::shared_ptr<ResourceManager> m_resource_manager;
        vk::PresentModeKHR m_present_mode;
        vk::SurfaceFormatKHR m_surface_format;
        vk::SurfaceCapabilitiesKHR m_surface_capabilities;
        std::vector<SwapFrame> m_swap_frame_pool;
        std::vector<SwapContext> m_swap_context_pool;
        QueueSupport m_queue_support;

    public:
        Swapchain(vk::raii::SwapchainKHR swapchain,
                  std::shared_ptr<vk::raii::Device> logical_device,
                  std::shared_ptr<ResourceManager> resource_manager,
                  vk::PresentModeKHR present_mode,
                  vk::SurfaceFormatKHR surface_format,
                  vk::SurfaceCapabilitiesKHR surface_capabilities,
                  std::vector<SwapFrame> swap_frame_pool,
                  std::vector<SwapContext> swap_context_pool,
                  QueueSupport queue_support)
            : m_swapchain(std::move(swapchain))
            , m_device(std::move(logical_device))
            , m_resource_manager(std::move(resource_manager))
            , m_present_mode(present_mode)
            , m_surface_format(surface_format)
            , m_surface_capabilities(surface_capabilities)
            , m_swap_frame_pool(std::move(swap_frame_pool))
            , m_swap_context_pool(std::move(swap_context_pool))
            , m_queue_support(std::move(queue_support))
        {};

        ~Swapchain() = default;

        VX::Expected<SwapState> try_acquire_next_swap_state();
        VX::Expected<void> try_present_and_swap(const SwapState&);
    };
}
