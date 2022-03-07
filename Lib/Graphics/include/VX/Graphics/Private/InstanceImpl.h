#pragma once

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

#include <VX/Graphics/GraphicsPipeline.h>
#include <VX/Graphics/RenderPipeline.h>
#include <VX/Graphics/Swapchain.h>
#include <VX/Graphics/Private/RenderPipelineImpl.h>
#include <VX/Graphics/Private/Vulkan.h>

namespace VX::Graphics::Private {
    class InstanceImpl final {
        VX_MAKE_NONCOPYABLE(InstanceImpl);
        VX_DEFAULT_MOVABLE(InstanceImpl);
    private:
        std::vector<std::shared_ptr<void>> m_instance_resources;
        RenderPipeline m_render_pipeline;
        Swapchain m_swapchain;
        GraphicsPipeline m_example_graphics_pipeline;

    public:
        InstanceImpl(Swapchain swapchain,
                     RenderPipeline render_pipeline,
                     GraphicsPipeline example_graphics_pipeline,
                     std::vector<std::shared_ptr<void>> instance_resources)
            : m_instance_resources(std::move(instance_resources))
            , m_render_pipeline(std::move(render_pipeline))
            , m_swapchain(std::move(swapchain))
            , m_example_graphics_pipeline(std::move(example_graphics_pipeline))
        {}

        ~InstanceImpl() = default;

        [[nodiscard]] const RenderPipeline& render_pipeline() const { return m_render_pipeline; }
        RenderPipeline& render_pipeline() { return m_render_pipeline; }

        [[nodiscard]] const Swapchain& swapchain() const { return m_swapchain; }
        Swapchain& swapchain() { return m_swapchain; }

        const GraphicsPipeline& example_graphics_pipeline() const { return m_example_graphics_pipeline; }
        GraphicsPipeline& example_graphics_pipeline() { return m_example_graphics_pipeline; }
    };
}
