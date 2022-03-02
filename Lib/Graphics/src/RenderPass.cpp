#include <VX/Graphics/RenderPass.h>
#include <VX/Graphics/Private/RenderPassImpl.h>
#include <VX/Graphics/Private/Vulkan.h>
#include <VX/Graphics/Private/Wrappers.h>

namespace VX::Graphics {
    const CommandRecorder& RenderPass::command_recorder() const {
        return impl().command_recorder();
    }

    CommandRecorder& RenderPass::command_recorder() {
        return impl().command_recorder();
    }
}
