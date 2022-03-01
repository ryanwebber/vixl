#pragma once

#include <optional>
#include <vector>

#include <VX/Graphics/APIObject.h>
#include <VX/Graphics/RenderTarget.h>
#include <VX/Graphics/Synchronization.h>

namespace VX::Graphics {

    namespace Private {
        class FrameSequencerImpl;
        class FrameSynchronizerImpl;
        class SwapchainImpl;
        class SwapchainTargetImpl;
    }

    class SwapchainTarget final: APIObject<Private::SwapchainTargetImpl> {
    public:
        using APIObject<Private::SwapchainTargetImpl>::APIObject;

        [[nodiscard]] int swap_index() const;
        [[nodiscard]] const RenderTarget& render_target() const;
        [[nodiscard]] const std::vector<std::shared_ptr<Semaphore>>& wait_semaphores() const;
        [[nodiscard]] const std::vector<std::shared_ptr<Semaphore>>& signal_semaphores() const;
        [[nodiscard]] const std::vector<std::shared_ptr<Fence>>& resource_reuse_fences() const;
    };

    class FrameSynchronizer final: APIObject<Private::FrameSynchronizerImpl> {
    public:
        using APIObject<Private::FrameSynchronizerImpl>::APIObject;

        void swap_and_present(const SwapchainTarget&);
    };

    class FrameSequencer final: APIObject<Private::FrameSequencerImpl> {
    public:
        using APIObject<Private::FrameSequencerImpl>::APIObject;

        std::optional<SwapchainTarget> acquire_next_swap_target();
    };

    class Swapchain final : public APIObject<Private::SwapchainImpl> {
    public:
        using APIObject<Private::SwapchainImpl>::APIObject;

        FrameSynchronizer& frame_synchronizer();
        [[nodiscard]] const FrameSynchronizer& frame_synchronizer() const;

        FrameSequencer& frame_sequencer();
        [[nodiscard]] const FrameSequencer& frame_sequencer() const;
    };
}
