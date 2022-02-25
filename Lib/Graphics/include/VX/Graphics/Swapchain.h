#pragma once

#include <vector>

#include <VX/Graphics/APIObject.h>
#include <VX/Graphics/Framebuffer.h>

namespace VX::Graphics {

    namespace Private {
        class SwapchainImpl;
    }

    class Swapchain final : public APIObject<Private::SwapchainImpl> {
    public:
        using APIObject<Private::SwapchainImpl>::APIObject;

        [[nodiscard]] const std::vector<std::shared_ptr<Framebuffer>> &framebuffers() const;
    };

    class FrameSynchronizer final {
    public:
        explicit FrameSynchronizer(const Swapchain& swapchain);
        ~FrameSynchronizer() = default;

        void swap_and_present(size_t index);
    };

    class FrameSequencer final {
    public:
        explicit FrameSequencer(const Swapchain& swapchain);
        ~FrameSequencer() = default;

        size_t next_swap_index();
    };
}
