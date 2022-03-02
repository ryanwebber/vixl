#pragma once

#include <memory>

#include <VX/Graphics/CommandBuffer.h>
#include <VX/Graphics/RenderTarget.h>
#include <VX/Graphics/Synchronization.h>

namespace VX::Graphics {
    struct RenderRequest {
        RenderTarget render_target;
        std::shared_ptr<Fence> render_fence;
        std::shared_ptr<Semaphore> wait_semaphore;
        std::shared_ptr<Semaphore> signal_semaphore;
        std::shared_ptr<CommandBuffer> command_buffer;
    };
}
