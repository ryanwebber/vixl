#include <VX/Graphics/RenderPass.h>
#include <VX/Graphics/Private/RenderPassImpl.h>
#include <VX/Graphics/Private/Vulkan.h>
#include <VX/Graphics/Private/Wrappers.h>

namespace VX::Graphics {

    namespace Private {
        void RenderPassImpl::submit() {
            vk::PipelineStageFlags wait_stage_flags[] = {
                vk::PipelineStageFlagBits::eColorAttachmentOutput
            };

            vk::Semaphore signal_semaphores[] = {
                ***m_render_timing.m_signal_semaphore
            };

            vk::Semaphore wait_semaphores[] = {
                ***m_render_timing.m_wait_semaphore
            };

            vk::CommandBuffer command_buffers[] = {
                ***command_recorder().command_buffer()
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
            queue.submit({ submit_info }, ***m_render_timing.m_render_fence);
        }
    }

    const CommandRecorder& RenderPass::command_recorder() const {
        return impl().command_recorder();
    }

    CommandRecorder& RenderPass::command_recorder() {
        return impl().command_recorder();
    }
}
