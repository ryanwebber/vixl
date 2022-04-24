#include <VX/Graphics/Assert.h>
#include <VX/Graphics/Swapchain.h>

namespace VX::Graphics {

    static constexpr auto k_semaphore_wait_timeout = std::numeric_limits<uint64_t>::max();

    RenderContext Swapchain::SwapState::to_render_context()
    {
        return {
                .command_buffer = command_buffer,
                .signal_fence = in_flight_fence,
                .wait_semaphores = { image_available_semaphore },
                .signal_semaphores = { render_finished_semaphore },
        };
    }

    VX::Expected<Swapchain::SwapState> Swapchain::try_acquire_next_swap_state() {
        VX_GRAPHICS_ASSERT(m_swap_index.acquire_index == m_swap_index.present_index,
                           "Swap indexes are out of sync: acquire({}) != present({})",
                           m_swap_index.acquire_index, m_swap_index.present_index);

        auto &swap_context = m_swap_context_pool[m_swap_index.acquire_index];
        auto wait_result = m_device->waitForFences({ **swap_context.in_flight_fence }, true, k_semaphore_wait_timeout);
        if (wait_result != vk::Result::eSuccess) {
            return VX::make_unexpected("Swap resources unavailable: {}", vk::to_string(wait_result));
        }

        m_device->resetFences({ **swap_context.in_flight_fence });

        auto &image_ready_semaphore = swap_context.image_available_semaphore;
        auto [image_acquisition_result, image_index] = m_swapchain.acquireNextImage(k_semaphore_wait_timeout, **image_ready_semaphore, nullptr);
        if (image_acquisition_result != vk::Result::eSuccess) {
            return VX::make_unexpected("Swap resources unavailable: {}", vk::to_string(image_acquisition_result));
        }

        VX_GRAPHICS_ASSERT(image_index < m_swap_frame_targets.size(), "No framebuffer created for swap index: {}", image_index);

        auto &target = m_swap_frame_targets[image_index];
        m_swap_index.image_index = image_index;
        m_swap_index.acquire_index = (m_swap_index.acquire_index + 1) % m_swap_context_pool.size();

        return SwapState {
                .render_target = target,
                .in_flight_fence = swap_context.in_flight_fence,
                .image_available_semaphore = swap_context.image_available_semaphore,
                .render_finished_semaphore = swap_context.render_finished_semaphore,
                .command_buffer = swap_context.command_buffer,
        };
    }

    VX::Expected<void> Swapchain::try_present_and_swap() {

        auto &swap_context = m_swap_context_pool[m_swap_index.present_index];

        vk::Semaphore wait_semaphores[] = {
            **swap_context.render_finished_semaphore,
        };

        vk::SwapchainKHR swapchains[] = {
            *m_swapchain
        };

        uint32_t swap_indexes[] = {
            static_cast<uint32_t>(m_swap_index.image_index),
        };

        vk::PresentInfoKHR present_info = {
                .waitSemaphoreCount = 1,
                .pWaitSemaphores = wait_semaphores,
                .swapchainCount = 1,
                .pSwapchains = swapchains,
                .pImageIndices = swap_indexes,
        };

        m_swap_index.present_index = (m_swap_index.present_index + 1) % m_swap_context_pool.size();

        auto queue = m_device->getQueue(m_queue_support.get_queue<QueueFeature::Presentation>(), 0);
        auto present_result = queue.presentKHR(present_info);

        if (present_result != vk::Result::eSuccess) {
            return VX::make_unexpected("Failed to present frame: {}", vk::to_string(present_result));
        }

        return { };
    }
}
