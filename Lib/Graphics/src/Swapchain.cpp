#include <limits>

#include <VX/Graphics/Swapchain.h>
#include <VX/Graphics/Private/Assert.h>
#include <VX/Graphics/Private/Logger.h>
#include <VX/Graphics/Private/SwapchainImpl.h>
#include <VX/Graphics/Private/Vulkan.h>
#include <VX/Graphics/Private/Wrappers.h>

namespace VX::Graphics {
    namespace Private {

        const auto semaphore_wait_timeout = std::numeric_limits<uint64_t>::max();

        RenderRequest SwapStateImpl::create_render_request() const {
            return {
                    .render_target = m_render_target,
                    .render_fence = m_render_fence,
                    .wait_semaphore = m_wait_semaphore,
                    .signal_semaphore = m_signal_semaphore,
                    .command_buffer = m_command_buffer
            };
        }

        void FrameSynchronizerImpl::swap_and_present(const SwapState &) {
            // TODO
        }

        FrameSequencerImpl::FrameSequencerImpl(std::shared_ptr<vk::raii::Device> device,
                                               std::shared_ptr<vk::raii::SwapchainKHR> swapchain,
                                               std::vector<std::shared_ptr<Framebuffer>> framebuffers,
                                               std::vector<std::shared_ptr<CommandBuffer>> command_buffers)
            : m_device(std::move(device))
            , m_swapchain(std::move(swapchain))
            , m_framebuffers(std::move(framebuffers))
            , m_command_buffers(std::move(command_buffers))
        {
            VX_GRAPHICS_ASSERT(m_framebuffers.size() >= 2, "Need at least 2 framebuffers to use for double buffering, got {}", m_framebuffers.size());
            VX_GRAPHICS_ASSERT(m_command_buffers.size() >= 2, "Need at least 2 command buffers to use for double buffering, got {}", m_command_buffers.size());

            const auto semaphore_count = max_inflight_count();
            m_image_selection_semaphores.reserve(semaphore_count);
            m_render_submission_semaphores.reserve(semaphore_count);
            m_frame_drawing_fences.reserve(semaphore_count);

            vk::SemaphoreCreateInfo semaphore_create_info = { };
            vk::FenceCreateInfo fence_create_info = { .flags = vk::FenceCreateFlagBits::eSignaled };

            for (auto i = 0; i < semaphore_count; i++) {
                m_image_selection_semaphores.push_back(std::make_shared<Semaphore>(*m_device, semaphore_create_info));
                m_render_submission_semaphores.push_back(std::make_shared<Semaphore>(*m_device, semaphore_create_info));
                m_frame_drawing_fences.push_back(std::make_shared<Fence>(*m_device, fence_create_info));
            }
        }

        std::optional<SwapState> FrameSequencerImpl::acquire_next_swap_state() {
            auto wait_result = m_device->waitForFences({ ***m_frame_drawing_fences[m_current_frame_index] }, true, semaphore_wait_timeout);
            if (wait_result != vk::Result::eSuccess) {
                Log::warn("Next swap target is unavailable: {}", vk::to_string(wait_result));
                return {};
            }

            m_device->resetFences({ ***m_frame_drawing_fences[m_current_frame_index] });

            auto [image_acquisition_result, image_index] = m_swapchain->acquireNextImage(semaphore_wait_timeout, ***m_image_selection_semaphores[m_current_frame_index], nullptr);
            if (image_acquisition_result != vk::Result::eSuccess) {
                Log::warn("Unable to acquire next swapchain image: {}", vk::to_string(image_acquisition_result));
                return {};
            }

            const auto& selected_command_buffer = m_command_buffers[m_current_frame_index];

            auto render_target = RenderTarget(m_framebuffers[image_index], { 800, 600 } ); // TODO: dimensions are wrong
            std::shared_ptr<Fence> render_fence = m_frame_drawing_fences[m_current_frame_index];
            std::shared_ptr<Semaphore> wait_semaphore = m_image_selection_semaphores[m_current_frame_index];
            std::shared_ptr<Semaphore> signal_semaphore = m_render_submission_semaphores[m_current_frame_index];

            m_current_frame_index = (m_current_frame_index + 1) % max_inflight_count();

            return SwapState(static_cast<int>(image_index), render_target, render_fence, wait_semaphore, signal_semaphore, selected_command_buffer);
        }
    }

    RenderRequest SwapState::create_render_request() const {
        return impl().create_render_request();
    }

    int SwapState::swap_index() const {
        return impl().swap_index();
    }

    void FrameSynchronizer::swap_and_present(const SwapState &swap_state) {
        return impl().swap_and_present(swap_state);
    }

    std::optional<SwapState> FrameSequencer::acquire_next_swap_state() {
        return impl().acquire_next_swap_state();
    }

    FrameSynchronizer& Swapchain::frame_synchronizer() {
        return impl().frame_synchronizer();
    }

    const FrameSynchronizer& Swapchain::frame_synchronizer() const {
        return impl().frame_synchronizer();
    }

    FrameSequencer& Swapchain::frame_sequencer() {
        return impl().frame_sequencer();
    }

    const FrameSequencer& Swapchain::frame_sequencer() const {
        return impl().frame_sequencer();
    }
}
