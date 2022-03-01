#pragma once

#include <VX/Graphics/APIObject.h>
#include <VX/Graphics/CommandRecorder.h>
#include <VX/Graphics/Synchronization.h>

namespace VX::Graphics {

    namespace Private {
        class RenderPassImpl;
    }

    struct RenderTiming {
        std::shared_ptr<Semaphore> m_wait_semaphore;
        std::shared_ptr<Semaphore> m_signal_semaphore;
        std::shared_ptr<Fence> m_render_fence;
    };

    class RenderPass final: public APIObject<Private::RenderPassImpl> {
    public:
        using APIObject<Private::RenderPassImpl>::APIObject;

        [[nodiscard]] const CommandRecorder& command_recorder() const;
        CommandRecorder& command_recorder();
    };
}
