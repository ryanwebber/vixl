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

        SwapchainTargetImpl::SwapchainTargetImpl(int swap_index,
                                                 RenderTarget render_target,
                                                 std::vector<std::shared_ptr<Semaphore>> wait_semaphores,
                                                 std::vector<std::shared_ptr<Semaphore>> signal_semaphores,
                                                 std::vector<std::shared_ptr<Fence>> resource_reuse_fences)
            : m_swap_index(swap_index)
            , m_render_target(std::move(render_target))
            , m_wait_semaphores(std::move(wait_semaphores))
            , m_signal_semaphores(std::move(signal_semaphores))
            , m_resource_reuse_fences(std::move(resource_reuse_fences))
        {
        }

        void FrameSynchronizerImpl::swap_and_present(const SwapchainTarget&) {
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

        std::optional<SwapchainTarget> FrameSequencerImpl::acquire_next_swap_target() {
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

            auto render_target = RenderTarget(m_framebuffers[image_index]);
            std::vector<std::shared_ptr<Semaphore>> wait_semaphores = { m_image_selection_semaphores[m_current_frame_index] };
            std::vector<std::shared_ptr<Semaphore>> signal_semaphores = { m_render_submission_semaphores[m_current_frame_index] };
            std::vector<std::shared_ptr<Fence>> resource_reuse_fences = { m_frame_drawing_fences[m_current_frame_index] };

            m_current_frame_index = (m_current_frame_index + 1) % max_inflight_count();

            return SwapchainTarget(static_cast<int>(image_index), render_target, signal_semaphores, wait_semaphores, resource_reuse_fences);
        }
    }

    int SwapchainTarget::swap_index() const {
        return impl().swap_index();
    }

    const RenderTarget& SwapchainTarget::render_target() const {
        return impl().render_target();
    }

    const std::vector<std::shared_ptr<Semaphore>>& SwapchainTarget::wait_semaphores() const {
        return impl().wait_semaphores();
    }

    const std::vector<std::shared_ptr<Semaphore>>& SwapchainTarget::signal_semaphores() const {
        return impl().signal_semaphores();
    }

    const std::vector<std::shared_ptr<Fence>>& SwapchainTarget::resource_reuse_fences() const {
        return impl().resource_reuse_fences();
    }

    void FrameSynchronizer::swap_and_present(const SwapchainTarget& target) {
        impl().swap_and_present(target);
    }

    std::optional<SwapchainTarget> FrameSequencer::acquire_next_swap_target() {
        return impl().acquire_next_swap_target();
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
