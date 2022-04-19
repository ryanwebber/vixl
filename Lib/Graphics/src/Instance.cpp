#include <VX/Graphics/Assert.h>
#include <VX/Graphics/Graphics.h>
#include <VX/Graphics/GraphicsPipeline.h>
#include <VX/Graphics/Instance.h>

namespace VX::Graphics {

    static constexpr size_t g_maximum_semaphores = 4;

    Instance::Instance(std::shared_ptr<InstanceImpl> impl)
        : m_impl(std::move(impl))
    {}

    Instance::Instance(Instance&&) noexcept = default;
    Instance& Instance::operator=(Instance&&) noexcept = default;

    Instance::Instance(const Instance&) noexcept = default;
    Instance& Instance::operator=(const Instance&) noexcept = default;

    Instance::~Instance() = default;

    const InstanceImpl* Instance::operator->() const { return m_impl.get(); }
    InstanceImpl* Instance::operator->() { return m_impl.get(); }

    const InstanceImpl &Instance::operator*() const { return *m_impl; }
    InstanceImpl &Instance::operator*() { return *m_impl; }

    VX::Expected<RenderTargetHandle> Instance::create_render_target(glm::vec<2, int> size)
    {
        return VX::make_unexpected("Not implemented");
    }

    VX::Expected<GraphicsPipelineHandle> Instance::create_graphics_pipeline(const GraphicsPipelineBuilder &)
    {
        // TODO: accurately create a pipeline
        GraphicsStage stage = {
                .identifier = 1,
                .info = {
                        .user_data = { }
                }
        };

        std::vector<std::unique_ptr<GraphicsStageExecutor>> executors;
        executors.push_back(std::make_unique<RenderFrameStage>(stage));

        auto& allocator = m_impl->resource_manager().handle_allocators().graphics_pipeline;
        return allocator.allocate<HandleType::GraphicsPipeline>(std::move(executors));
    }

    VX::Expected<void> Instance::execute_graphics_pipeline(const GraphicsPipelineHandle &handle, const GraphicsDelegate &delegate)
    {
        auto& allocator = m_impl->resource_manager().handle_allocators().graphics_pipeline;
        return allocator.lookup(handle).execute(*m_impl, delegate);
    }

    // Instance Impl

    VX::Expected<RenderPass> InstanceImpl::begin_render_pass(const RenderTarget &target, const RenderContext &context)
    {
        vk::CommandBufferBeginInfo command_buffer_begin_info = {};

        vk::ClearValue clear_values[] = {
                {
                        .color = {{{ 0.0f, 0.0f, 0.0f, 0.0f }}},
                }
        };

        vk::RenderPassBeginInfo render_pass_begin_info = {
                .renderPass = &(**m_render_pass),
                .framebuffer = &(**target.framebuffer()),
                .renderArea = {
                        .offset = {
                                .x = 0,
                                .y = 0,
                        },
                        .extent = target.extents()
                },
                .clearValueCount = 1,
                .pClearValues = clear_values
        };

        auto& command_buffer = *context.command_buffer;
        command_buffer.reset();
        command_buffer.begin(command_buffer_begin_info);
        command_buffer.beginRenderPass(render_pass_begin_info, vk::SubpassContents::eInline);

        return RenderPass(context);
    }

    VX::Expected<void> InstanceImpl::end_render_pass(const RenderPass &render_pass)
    {
        vk::PipelineStageFlags wait_stage_flags[] = {
                vk::PipelineStageFlagBits::eColorAttachmentOutput
        };

        vk::Semaphore signal_semaphores[g_maximum_semaphores];
        auto num_signal_semaphores = render_pass.render_context().m_signal_semaphores.size();
        VX_GRAPHICS_ASSERT(num_signal_semaphores <= g_maximum_semaphores,
                           "Too many signal semaphores requested: {} > {}", num_signal_semaphores, g_maximum_semaphores);
        for (auto i = 0; i < std::min(g_maximum_semaphores, num_signal_semaphores); i++)
            signal_semaphores[i] = **render_pass.render_context().m_signal_semaphores[i];

        vk::Semaphore wait_semaphores[g_maximum_semaphores];
        auto num_wait_semaphores = render_pass.render_context().m_wait_semaphores.size();
        VX_GRAPHICS_ASSERT(num_wait_semaphores <= g_maximum_semaphores,
                           "Too many wait semaphores requested: {} > {}", num_wait_semaphores, g_maximum_semaphores);
        for (auto i = 0; i < std::min(g_maximum_semaphores, num_wait_semaphores); i++)
            wait_semaphores[i] = **render_pass.render_context().m_wait_semaphores[i];

        vk::CommandBuffer command_buffers[] = {
                **render_pass.render_context().command_buffer
        };

        vk::SubmitInfo submit_info = {
                .waitSemaphoreCount = static_cast<uint32_t>(num_wait_semaphores),
                .pWaitSemaphores = wait_semaphores,
                .pWaitDstStageMask = wait_stage_flags,
                .commandBufferCount = 1,
                .pCommandBuffers = command_buffers,
                .signalSemaphoreCount = static_cast<uint32_t>(num_signal_semaphores),
                .pSignalSemaphores = signal_semaphores,
        };

        auto queue = m_logical_device->getQueue(m_queue_support.get_queue<QueueFeature::Graphics>(), 0);
        queue.submit({ submit_info }, **render_pass.render_context().m_signal_fence);

        render_pass.render_context().command_buffer->endRenderPass();
        render_pass.render_context().command_buffer->end();

        return {};
    }
}
