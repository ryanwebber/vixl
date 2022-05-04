#include <VX/Noncopyable.h>
#include <VX/Graphics/Graphics.h>
#include <VX/Graphics/GraphicsContextImpl.h>
#include <VX/Graphics/GraphicsPipeline.h>
#include <VX/Graphics/Instance.h>
#include <VX/Graphics/RenderPass.h>

namespace VX::Graphics {

    VX::Expected<void> RenderFrameStage::execute(InstanceImpl &instance, GraphicsClient &client) {
        auto maybe_swap_state = instance.swapchain().try_acquire_next_swap_state();
        if (!maybe_swap_state)
            return VX::erase(maybe_swap_state);

        auto render_target = maybe_swap_state->render_target;
        auto render_context = maybe_swap_state->to_render_context();
        auto maybe_render_pass = instance.begin_render_pass(*render_target, render_context);
        if (!maybe_render_pass)
            return VX::erase(maybe_render_pass);

        GraphicsContextImpl graphics_context(instance, maybe_render_pass.value());
        client.do_render(graphics_stage(), graphics_context);

        auto maybe_render_pass_end = instance.end_render_pass(maybe_render_pass.value());
        if (!maybe_render_pass_end)
            return VX::erase(maybe_render_pass_end);

        auto present_end = instance.swapchain().try_present_and_swap();
        if (!present_end)
            return VX::erase(present_end);

        return { };
    }
}
