#pragma once

#include <unordered_map>
#include <vector>

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

#include <VX/Graphics/Assert.h>
#include <VX/Graphics/Graphics.h>
#include <VX/Graphics/QueueSupport.h>
#include <VX/Graphics/RenderContext.h>
#include <VX/Graphics/RenderTarget.h>
#include <VX/Graphics/Swapchain.h>
#include <VX/Graphics/Vulkan.h>

namespace VX::Graphics {

    template <class T>
    using ResourceList = std::unordered_map<HandleIdentifier, T>;

    class InstanceImpl final {
        VX_MAKE_NONCOPYABLE(InstanceImpl);
        VX_MAKE_NONMOVABLE(InstanceImpl);
    private:
        vk::raii::Context m_context;
        vk::raii::Instance m_instance;
        std::vector<std::shared_ptr<void>> m_callbacks;
        vk::raii::SurfaceKHR m_surface;
        vk::raii::PhysicalDevice m_physical_device;
        vk::raii::Device m_logical_device;
        Swapchain m_swapchain;
        vk::raii::RenderPass m_render_pass;
        vk::raii::CommandPool m_command_pool;

        QueueSupport m_queue_support;

        ResourceList<RenderTarget> m_render_targets { };
        ResourceList<RenderContext> m_render_contexts { };
        ResourceList<vk::raii::CommandBuffer> m_command_buffers { };
        ResourceList<vk::raii::Pipeline> m_graphics_pipelines { };

    public:
        InstanceImpl(vk::raii::Context context,
                     vk::raii::Instance instance,
                     std::vector<std::shared_ptr<void>> callbacks,
                     vk::raii::SurfaceKHR surface,
                     vk::raii::PhysicalDevice physical_device,
                     vk::raii::Device logical_device,
                     Swapchain swapchain,
                     vk::raii::RenderPass render_pass,
                     vk::raii::CommandPool command_pool,
                     ResourceList<RenderTarget> render_targets,
                     ResourceList<RenderContext> render_contexts,
                     ResourceList<vk::raii::CommandBuffer> command_buffers,
                     ResourceList<vk::raii::Pipeline> graphics_pipelines,
                     QueueSupport queue_support)
            : m_context(std::move(context))
            , m_instance(std::move(instance))
            , m_callbacks(std::move(callbacks))
            , m_surface(std::move(surface))
            , m_physical_device(std::move(physical_device))
            , m_logical_device(std::move(logical_device))
            , m_swapchain(std::move(swapchain))
            , m_render_pass(std::move(render_pass))
            , m_command_pool(std::move(command_pool))
            , m_render_targets(std::move(render_targets))
            , m_render_contexts(std::move(render_contexts))
            , m_command_buffers(std::move(command_buffers))
            , m_graphics_pipelines(std::move(graphics_pipelines))
            , m_queue_support(std::move(queue_support))
        {}

        ~InstanceImpl() = default;

        template <HandleType T>
        void destroy_handle(const Handle<T>&) {
            VX_GRAPHICS_ASSERT_NOT_REACHED();
        }
    };

    template<>
    void InstanceImpl::destroy_handle(const Handle<HandleType::GraphicsPipeline> &handle);
}
