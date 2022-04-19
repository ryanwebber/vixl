#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

#include <VX/Graphics/Assert.h>
#include <VX/Graphics/Graphics.h>
#include <VX/Graphics/GraphicsPipeline.h>
#include <VX/Graphics/QueueSupport.h>
#include <VX/Graphics/RenderContext.h>
#include <VX/Graphics/RenderPass.h>
#include <VX/Graphics/RenderTarget.h>
#include <VX/Graphics/Memory.h>
#include <VX/Graphics/Swapchain.h>
#include <VX/Graphics/Vulkan.h>

namespace VX::Graphics {

    class ResourceManager final {
        VX_DEFAULT_MOVABLE(ResourceManager);
        VX_MAKE_NONCOPYABLE(ResourceManager);
    public:
        struct HandleAllocatorCollection {
            HandleAllocator<GraphicsPipeline> graphics_pipeline;

            HandleAllocatorCollection() = delete;
            explicit HandleAllocatorCollection(Allocator &backing_allocator)
                : graphics_pipeline(backing_allocator)
            {};
        };

    private:
        Allocator m_general_allocator;
        HandleAllocatorCollection m_handle_allocators;

    public:
        explicit ResourceManager()
            : m_general_allocator()
            , m_handle_allocators(m_general_allocator)
        {};

        explicit ResourceManager(Allocator allocator)
                : m_general_allocator(std::move(allocator))
                , m_handle_allocators(m_general_allocator)
        {};

        HandleAllocatorCollection &handle_allocators() {
            return m_handle_allocators;
        }

        [[nodiscard]] const HandleAllocatorCollection &handle_allocators() const {
            return m_handle_allocators;
        }

        Allocator &allocator() { return m_general_allocator; }
        [[nodiscard]] const Allocator &allocator() const { return m_general_allocator; }

        ~ResourceManager() = default;
    };

    class InstanceImpl final {
        VX_MAKE_NONCOPYABLE(InstanceImpl);
        VX_MAKE_NONMOVABLE(InstanceImpl);
    private:
        vk::raii::Context m_context;
        vk::raii::Instance m_instance;
        std::vector<std::shared_ptr<void>> m_callbacks;
        vk::raii::SurfaceKHR m_surface;
        vk::raii::PhysicalDevice m_physical_device;
        std::shared_ptr<vk::raii::Device> m_logical_device;
        vk::raii::CommandPool m_command_pool;
        Swapchain m_swapchain;
        vk::raii::RenderPass m_render_pass;
        ResourceManager m_resource_manager;
        QueueSupport m_queue_support;

    public:
        InstanceImpl(vk::raii::Context context,
                     vk::raii::Instance instance,
                     std::vector<std::shared_ptr<void>> callbacks,
                     vk::raii::SurfaceKHR surface,
                     vk::raii::PhysicalDevice physical_device,
                     std::shared_ptr<vk::raii::Device> logical_device,
                     vk::raii::CommandPool command_pool,
                     Swapchain swapchain,
                     vk::raii::RenderPass render_pass,
                     ResourceManager resource_manager,
                     QueueSupport queue_support)
            : m_context(std::move(context))
            , m_instance(std::move(instance))
            , m_callbacks(std::move(callbacks))
            , m_surface(std::move(surface))
            , m_physical_device(std::move(physical_device))
            , m_logical_device(std::move(logical_device))
            , m_command_pool(std::move(command_pool))
            , m_swapchain(std::move(swapchain))
            , m_render_pass(std::move(render_pass))
            , m_resource_manager(std::move(resource_manager))
            , m_queue_support(std::move(queue_support))
        {}

        ~InstanceImpl() = default;

        ResourceManager &resource_manager() { return m_resource_manager; }
        [[nodiscard]] const ResourceManager &resource_manager() const { return m_resource_manager; }

        Swapchain &swapchain() { return m_swapchain; }
        [[nodiscard]] const Swapchain &swapchain() const { return m_swapchain; }

        VX::Expected<RenderPass> begin_render_pass(const RenderTarget&, const RenderContext&);
        VX::Expected<void> end_render_pass(const RenderPass&);
    };
}
