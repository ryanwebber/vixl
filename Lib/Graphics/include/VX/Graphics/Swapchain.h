#pragma once

#include <vector>

#include <VX/Graphics/APIObject.h>
#include <VX/Graphics/RenderTarget.h>

namespace VX::Graphics {

    namespace Private {
        class SwapchainImpl;
    }

    class Swapchain final : public APIObject<Private::SwapchainImpl> {
    public:
        using APIObject<Private::SwapchainImpl>::APIObject;
    };

    struct SwapchainTarget {
        RenderTarget render_target;
        int swap_index;
    };

    class FrameSynchronizer final {
    public:

    public:
        ~FrameSynchronizer() = default;

        void swap_and_present(const SwapchainTarget&);
    };

    class FrameSequencer final {
    public:
        ~FrameSequencer() = default;

        RenderTarget next_render_target();
    };
}
