#include <VX/Graphics/RenderPipeline.h>
#include <VX/Graphics/Private/RenderPipelineImpl.h>
#include <VX/Graphics/Private/Wrappers.h>
#include <VX/Graphics/Private/Vulkan.h>

namespace VX::Graphics {

    namespace Private {

        std::optional<RenderPass> RenderPipelineImpl::try_begin_render_pass(RenderRequest request) {

            vk::CommandBufferBeginInfo command_buffer_begin_info = {};

            vk::ClearValue clear_values[] = {
                    {
                        .color = {{{ 0.0f, 0.0f, 0.0f, 0.0f }}},
                    }
            };

            vk::RenderPassBeginInfo render_pass_begin_info = {
                    .renderPass = **m_render_pass,
                    .framebuffer = ***request.render_target.target_framebuffer(),
                    .renderArea = {
                            .offset = {
                                    .x = 0,
                                    .y = 0,
                            },
                            .extent = {
                                    .width = static_cast<uint32_t>(request.render_target.dimensions().x),
                                    .height = static_cast<uint32_t>(request.render_target.dimensions().y),
                            },
                    },
                    .clearValueCount = 1,
                    .pClearValues = clear_values
            };

            request.command_buffer->impl()->reset();
            request.command_buffer->impl()->begin(command_buffer_begin_info);
            request.command_buffer->impl()->beginRenderPass(render_pass_begin_info, vk::SubpassContents::eInline);

            CommandRecorder command_recorder(request.command_buffer);
            return RenderPass(command_recorder,
                              request.render_target,
                              request.render_fence,
                              request.wait_semaphore,
                              request.signal_semaphore);
        }

        void RenderPipelineImpl::end_render_pass(RenderPass &render_pass) {
            vk::PipelineStageFlags wait_stage_flags[] = {
                    vk::PipelineStageFlagBits::eColorAttachmentOutput
            };

            vk::Semaphore signal_semaphores[] = {
                    **(render_pass->signal_semaphore())
            };

            vk::Semaphore wait_semaphores[] = {
                    **(render_pass->wait_semaphore())
            };

            vk::CommandBuffer command_buffers[] = {
                    **(render_pass->command_recorder().command_buffer())
            };

            vk::SubmitInfo submit_info = {
                    .waitSemaphoreCount = 1,
                    .pWaitSemaphores = wait_semaphores,
                    .pWaitDstStageMask = wait_stage_flags,
                    .commandBufferCount = 1,
                    .pCommandBuffers = command_buffers,
                    .signalSemaphoreCount = 1,
                    .pSignalSemaphores = signal_semaphores,
            };

            auto queue = m_device->getQueue(m_queue_support.get_queue<QueueFeature::Graphics>(), 0);
            queue.submit({ submit_info }, **(render_pass->render_fence()));

            render_pass.command_recorder().command_buffer().impl()->endRenderPass();
            render_pass.command_recorder().command_buffer().impl()->end();
        }
    }

    std::optional<RenderPass> RenderPipeline::try_begin_render_pass(RenderRequest request) {
        return impl().try_begin_render_pass(std::move(request));
    }

    void RenderPipeline::end_render_pass(RenderPass &render_pass) {
        impl().end_render_pass(render_pass);
    }
}
