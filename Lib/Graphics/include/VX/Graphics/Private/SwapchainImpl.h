#pragma once

#include <optional>
#include <vector>

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

#include <VX/Graphics/Framebuffer.h>
#include <VX/Graphics/Swapchain.h>
#include <VX/Graphics/Private/Vulkan.h>

namespace VX::Graphics::Private {

    class SwapStateImpl final {
        VX_MAKE_NONCOPYABLE(SwapStateImpl);
        VX_DEFAULT_MOVABLE(SwapStateImpl);
    private:
        int m_swap_index;
        RenderTarget m_render_target;
        std::shared_ptr<Fence> m_render_fence;
        std::shared_ptr<Semaphore> m_wait_semaphore;
        std::shared_ptr<Semaphore> m_signal_semaphore;
        std::shared_ptr<CommandBuffer> m_command_buffer;

    public:
        SwapStateImpl(int swap_index,
                      RenderTarget render_target,
                      std::shared_ptr<Fence> render_fence,
                      std::shared_ptr<Semaphore> wait_semaphore,
                      std::shared_ptr<Semaphore> signal_semaphore,
                      std::shared_ptr<CommandBuffer> command_buffer)
            : m_swap_index(swap_index)
            , m_render_target(std::move(render_target))
            , m_render_fence(std::move(render_fence))
            , m_wait_semaphore(std::move(wait_semaphore))
            , m_signal_semaphore(std::move(signal_semaphore))
            , m_command_buffer(std::move(command_buffer))
        {}

        [[nodiscard]] RenderRequest create_render_request() const;
        [[nodiscard]] int swap_index() const { return m_swap_index; }
    };

    class FrameSequencerImpl final {
        VX_MAKE_NONCOPYABLE(FrameSequencerImpl);
        VX_DEFAULT_MOVABLE(FrameSequencerImpl);
    private:
        size_t m_current_frame_index { 0 };
        std::shared_ptr<vk::raii::Device> m_device;
        std::shared_ptr<vk::raii::SwapchainKHR> m_swapchain;
        std::vector<std::shared_ptr<Framebuffer>> m_framebuffers;
        std::vector<std::shared_ptr<CommandBuffer>> m_command_buffers;
        std::vector<std::shared_ptr<Semaphore>> m_image_selection_semaphores;
        std::vector<std::shared_ptr<Semaphore>> m_render_submission_semaphores;
        std::vector<std::shared_ptr<Fence>> m_frame_drawing_fences;

        [[nodiscard]] size_t image_count() const { return m_framebuffers.size(); }
        [[nodiscard]] size_t max_inflight_count() const { return m_command_buffers.size(); }

    public:
        explicit FrameSequencerImpl(std::shared_ptr<vk::raii::Device>,
                                    std::shared_ptr<vk::raii::SwapchainKHR>,
                                    std::vector<std::shared_ptr<Framebuffer>>,
                                    std::vector<std::shared_ptr<CommandBuffer>>);

        std::optional<SwapState> acquire_next_swap_state();
    };

    class FrameSynchronizerImpl final {
        VX_MAKE_NONCOPYABLE(FrameSynchronizerImpl);
        VX_DEFAULT_MOVABLE(FrameSynchronizerImpl);
    private:
        std::shared_ptr<vk::raii::SwapchainKHR> m_swapchain;
    public:
        explicit FrameSynchronizerImpl(std::shared_ptr<vk::raii::SwapchainKHR> swapchain)
                : m_swapchain(std::move(swapchain))
        {}

        void swap_and_present(const SwapState&);
    };

    class SwapchainImpl final {
        VX_MAKE_NONCOPYABLE(SwapchainImpl);
        VX_DEFAULT_MOVABLE(SwapchainImpl);
    private:
        std::shared_ptr<vk::raii::SwapchainKHR> m_swapchain;
        std::vector<std::shared_ptr<Framebuffer>> m_framebuffers;
        std::vector<std::shared_ptr<CommandBuffer>> m_command_buffers;

        FrameSequencer m_frame_sequencer;
        FrameSynchronizer m_frame_synchronizer;

    public:
        SwapchainImpl(vk::raii::SwapchainKHR swapchain,
                      std::shared_ptr<vk::raii::Device> device,
                      std::vector<std::shared_ptr<Framebuffer>> framebuffers,
                      std::vector<std::shared_ptr<CommandBuffer>> command_buffers)
            : m_swapchain(std::make_shared<vk::raii::SwapchainKHR>(std::move(std::move(swapchain))))
            , m_framebuffers(std::move(framebuffers))
            , m_command_buffers(std::move(command_buffers))
            , m_frame_sequencer(device, m_swapchain, m_framebuffers, m_command_buffers)
            , m_frame_synchronizer(m_swapchain)
        {}

        FrameSynchronizer& frame_synchronizer() { return m_frame_synchronizer; }
        [[nodiscard]] const FrameSynchronizer& frame_synchronizer() const { return m_frame_synchronizer; }

        FrameSequencer& frame_sequencer() { return m_frame_sequencer; }
        [[nodiscard]] const FrameSequencer& frame_sequencer() const { return m_frame_sequencer; }
    };
}
