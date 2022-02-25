#include <VX/Graphics/Private/InstanceImpl.h>
#include <VX/Graphics/Private/RenderPassImpl.h>
#include <VX/Graphics/Private/SwapchainImpl.h>

#include <VX/Graphics/Instance.h>

namespace VX::Graphics {
    const RenderPipeline& Instance::render_pipeline() const {
        return impl().render_pipeline();
    }

    RenderPipeline& Instance::render_pipeline() {
        return impl().render_pipeline();
    }

    const Swapchain& Instance::swapchain() const {
        return impl().swapchain();
    }

    Swapchain& Instance::swapchain() {
        return impl().swapchain();
    }
}
