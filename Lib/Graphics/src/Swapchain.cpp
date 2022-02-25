#include <VX/Graphics/Swapchain.h>
#include <VX/Graphics/Private/SwapchainImpl.h>
#include <VX/Graphics/Private/Vulkan.h>

namespace VX::Graphics {
    const std::vector<std::shared_ptr<Framebuffer>> &Swapchain::framebuffers() const {
        return impl().framebuffers();
    }
}
