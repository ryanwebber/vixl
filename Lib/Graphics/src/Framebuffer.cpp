#include <VX/Graphics/Framebuffer.h>
#include <VX/Graphics/Private/FramebufferImpl.h>
#include <VX/Graphics/Private/Vulkan.h>

namespace VX::Graphics {
    std::shared_ptr<CommandBuffer> Framebuffer::associated_command_buffer() const {
        return impl().associated_command_buffer();
    }
}
