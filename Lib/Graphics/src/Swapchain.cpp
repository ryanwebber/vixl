#include <VX/Graphics/Assert.h>
#include <VX/Graphics/Swapchain.h>

namespace VX::Graphics {

    static constexpr auto semaphore_wait_timeout = std::numeric_limits<uint64_t>::max();

    VX::Expected<SwapState> Swapchain::try_acquire_next_swap_state() {
        auto &swap_context = m_swap_context_pool[m_frame_index];
        auto &render_context = m_resource_manager->render_contexts().lookup(*swap_context.render_context_handle());

        auto wait_result = m_device->waitForFences({ *render_context.render_fence() }, true, semaphore_wait_timeout);
        if (wait_result != vk::Result::eSuccess) {
            return VX::make_unexpected("Swap resources unavailable: {}", vk::to_string(wait_result));
        }

        m_device->resetFences({ *render_context.render_fence() });

        auto [image_acquisition_result, image_index] = m_swapchain.acquireNextImage(semaphore_wait_timeout, *render_context.wait_semaphore(), nullptr);
        if (image_acquisition_result != vk::Result::eSuccess) {
            return VX::make_unexpected("Swap resources unavailable: {}", vk::to_string(image_acquisition_result));
        }

        VX_GRAPHICS_ASSERT(image_index < m_swap_frame_pool.size(), "No framebuffer created for swap index: {}", image_index);

        auto &swap_frame = m_swap_frame_pool[image_index];

        m_frame_index = (m_frame_index + 1) % m_swap_context_pool.size();

        return SwapState {
            .swap_index = image_index,
            .context = swap_context.render_context_handle(),
            .frame_buffer = swap_frame.render_target_handle(),
            .command_buffer = swap_context.command_buffer_handle(),
        };
    }

    VX::Expected<void> Swapchain::try_present_and_swap(const SwapState &swap_state) {

        auto& render_context = m_resource_manager->render_contexts().lookup(*swap_state.context);

        vk::Semaphore wait_semaphores[] = {
            *render_context.signal_semaphore()
        };

        vk::SwapchainKHR swapchains[] = {
            *m_swapchain
        };

        uint32_t swap_indexes[] = {
            static_cast<uint32_t>(swap_state.swap_index)
        };

        vk::PresentInfoKHR present_info = {
                .waitSemaphoreCount = 1,
                .pWaitSemaphores = wait_semaphores,
                .swapchainCount = 1,
                .pSwapchains = swapchains,
                .pImageIndices = swap_indexes,
        };

        auto queue = m_device->getQueue(m_queue_support.get_queue<QueueFeature::Presentation>(), 0);
        auto present_result = queue.presentKHR(present_info);

        if (present_result != vk::Result::eSuccess) {
            return VX::make_unexpected("Failed to present frame: {}", vk::to_string(present_result));
        }

        return { };
    }
}
