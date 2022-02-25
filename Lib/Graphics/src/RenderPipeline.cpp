#include <VX/Graphics/RenderPipeline.h>
#include <VX/Graphics/Private/RenderPipelineImpl.h>
#include <VX/Graphics/Private/Vulkan.h>

namespace VX::Graphics {
    Token<RenderPass> RenderPipeline::try_begin_render_pass(const RenderTarget& render_target) {
        return impl().try_begin_render_pass(render_target);
    }

    void RenderPipeline::end_render_pass(Token<RenderPass> &render_pass) {
        impl().end_render_pass(render_pass);
    }
}
