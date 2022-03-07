#pragma once

#include <optional>
#include <vector>

#include <VX/Graphics/APIObject.h>
#include <VX/Graphics/RenderRequest.h>
#include <VX/Graphics/RenderTarget.h>
#include <VX/Graphics/Synchronization.h>

namespace VX::Graphics {

    namespace Private {
        class FrameSequencerImpl;
        class FrameSynchronizerImpl;
        class SwapchainImpl;
        class SwapStateImpl;
    }

    class SwapState final: public APIObject<Private::SwapStateImpl> {
    public:
        using APIObject<Private::SwapStateImpl>::APIObject;

        [[nodiscard]] RenderRequest create_render_request() const;
        [[nodiscard]] int swap_index() const;
    };

    class FrameSequencer final: APIObject<Private::FrameSequencerImpl> {
    public:
        using APIObject<Private::FrameSequencerImpl>::APIObject;

        std::optional<SwapState> acquire_next_swap_state();
    };

    class FrameSynchronizer final: APIObject<Private::FrameSynchronizerImpl> {
    public:
        using APIObject<Private::FrameSynchronizerImpl>::APIObject;

        void swap_and_present(const SwapState&);
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
