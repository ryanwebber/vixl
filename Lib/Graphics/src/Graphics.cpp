#include <VX/Graphics/Graphics.h>
#include <VX/Graphics/Instance.h>

namespace VX::Graphics {

    VX::Expected<void> begin_render_pass(Instance &instance,
                                         const RenderContextHandle &render_context_handle,
                                         const RenderTargetHandle &render_target_handle,
                                         const CommandBufferHandle &command_buffer_handle) {
        return instance->begin_render_pass(render_context_handle, render_target_handle, command_buffer_handle);
    }

    VX::Expected<void> end_render_pass(Instance &instance) {
        return instance->end_render_pass();
    }

    VX::Expected<SwapState> begin_frame(Instance &instance) {
        return instance->begin_frame();
    }

    VX::Expected<void> end_frame(Instance &instance, const SwapState &swap_state) {
        return instance->end_frame(swap_state);
    }

    void bind(Instance &instance, const GraphicsPipelineHandle &graphics_pipeline_handle) {
        return instance->bind(graphics_pipeline_handle);
    }

    void draw(Instance &instance) {
        return instance->draw();
    }
}
