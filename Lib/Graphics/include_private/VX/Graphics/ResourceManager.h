#pragma once

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

#include <VX/Graphics/Graphics.h>
#include <VX/Graphics/RenderTarget.h>
#include <VX/Graphics/RenderContext.h>

namespace VX::Graphics {
    class ResourceManager final {
        VX_MAKE_NONCOPYABLE(ResourceManager);
        VX_DEFAULT_MOVABLE(ResourceManager);
    private:
        std::shared_ptr<ResourceAllocator<RenderTarget, HandleType::RenderTarget>> m_render_targets;
        std::shared_ptr<ResourceAllocator<RenderContext, HandleType::RenderContext>> m_render_contexts;
        std::shared_ptr<ResourceAllocator<vk::raii::CommandBuffer, HandleType::CommandBuffer>> m_command_buffers;
        std::shared_ptr<ResourceAllocator<vk::raii::Pipeline, HandleType::GraphicsPipeline>> m_graphics_pipelines;
    public:
        ResourceManager()
            : m_render_targets(ResourceAllocator<RenderTarget, HandleType::RenderTarget>::create_shared())
            , m_render_contexts(ResourceAllocator<RenderContext, HandleType::RenderContext>::create_shared())
            , m_command_buffers(ResourceAllocator<vk::raii::CommandBuffer, HandleType::CommandBuffer>::create_shared())
            , m_graphics_pipelines(ResourceAllocator<vk::raii::Pipeline, HandleType::GraphicsPipeline>::create_shared())
        {}

        ~ResourceManager() = default;

        ResourceAllocator<RenderTarget, HandleType::RenderTarget> &render_targets() { return *m_render_targets; };
        [[nodiscard]] const ResourceAllocator<RenderTarget, HandleType::RenderTarget> &render_targets() const { return *m_render_targets; };

        ResourceAllocator<RenderContext, HandleType::RenderContext> &render_contexts() { return *m_render_contexts; };
        [[nodiscard]] const ResourceAllocator<RenderContext, HandleType::RenderContext> &render_contexts() const { return *m_render_contexts; };

        ResourceAllocator<vk::raii::CommandBuffer, HandleType::CommandBuffer> &command_buffers() { return *m_command_buffers; };
        [[nodiscard]] const ResourceAllocator<vk::raii::CommandBuffer, HandleType::CommandBuffer> &command_buffers() const { return *m_command_buffers; };

        ResourceAllocator<vk::raii::Pipeline, HandleType::GraphicsPipeline> &graphics_pipelines() { return *m_graphics_pipelines; };
        [[nodiscard]] const ResourceAllocator<vk::raii::Pipeline, HandleType::GraphicsPipeline> &graphics_pipelines() const { return *m_graphics_pipelines; };
    };
}
