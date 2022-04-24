#pragma once

#include <VX/Copyable.h>
#include <VX/Expected.h>
#include <VX/Noncopyable.h>

#include <VX/Graphics/Graphics.h>
#include <VX/Graphics/QueueSupport.h>
#include <VX/Graphics/RenderPass.h>
#include <VX/Graphics/RenderTarget.h>
#include <VX/Graphics/Memory.h>
#include <VX/Graphics/Vulkan.h>

namespace VX::Graphics {

    struct SwapIndex {
        uint32_t image_index { 0 };
        uint32_t acquire_index { 0 };
        uint32_t present_index { 0 };
    };

    class Swapchain final {
        VX_MAKE_NONCOPYABLE(Swapchain);
        VX_DEFAULT_MOVABLE(Swapchain);
    public:
        struct SwapContext final {
            InstancePtr<vk::raii::Fence> in_flight_fence;
            InstancePtr<vk::raii::Semaphore> image_available_semaphore;
            InstancePtr<vk::raii::Semaphore> render_finished_semaphore;
            InstancePtr<vk::raii::CommandBuffer> command_buffer;
        };

        struct SwapState final {
            InstancePtr<RenderTarget> render_target;
            InstancePtr<vk::raii::Fence> in_flight_fence;
            InstancePtr<vk::raii::Semaphore> image_available_semaphore;
            InstancePtr<vk::raii::Semaphore> render_finished_semaphore;
            InstancePtr<vk::raii::CommandBuffer> command_buffer;

            RenderContext to_render_context();
        };

    private:
        SwapIndex m_swap_index { };
        vk::raii::SwapchainKHR m_swapchain;
        std::shared_ptr<vk::raii::Device> m_device;
        vk::PresentModeKHR m_present_mode;
        vk::SurfaceFormatKHR m_surface_format;
        vk::SurfaceCapabilitiesKHR m_surface_capabilities;
        std::vector<InstancePtr<RenderTarget>> m_swap_frame_targets;
        std::vector<SwapContext> m_swap_context_pool;
        QueueSupport m_queue_support;

    public:
        Swapchain(vk::raii::SwapchainKHR swapchain,
                  std::shared_ptr<vk::raii::Device> logical_device,
                  vk::PresentModeKHR present_mode,
                  vk::SurfaceFormatKHR surface_format,
                  vk::SurfaceCapabilitiesKHR surface_capabilities,
                  std::vector<InstancePtr<RenderTarget>> swap_frame_targets,
                  std::vector<SwapContext> swap_context_pool,
                  QueueSupport queue_support)
            : m_swapchain(std::move(swapchain))
            , m_device(std::move(logical_device))
            , m_present_mode(present_mode)
            , m_surface_format(surface_format)
            , m_surface_capabilities(surface_capabilities)
            , m_swap_frame_targets(std::move(swap_frame_targets))
            , m_swap_context_pool(std::move(swap_context_pool))
            , m_queue_support(std::move(queue_support))
        {};

        ~Swapchain() = default;

        VX::Expected<SwapState> try_acquire_next_swap_state();
        VX::Expected<void> try_present_and_swap();
    };
}
