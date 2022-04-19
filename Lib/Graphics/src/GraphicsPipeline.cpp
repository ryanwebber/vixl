#include <VX/Graphics/GraphicsPipeline.h>
#include <VX/Graphics/Instance.h>

namespace VX::Graphics {
    RenderContext RenderFrameStage::render_context_from(const Swapchain::SwapState &) {
        // TODO: incorporate other stage semaphores and such
        VX_GRAPHICS_ASSERT_NOT_REACHED();
    }

    VX::Expected<void> RenderFrameStage::execute(InstanceImpl &instance, GraphicsClient &client) {

        auto maybe_swap_state = instance.swapchain().try_acquire_next_swap_state();
        if (!maybe_swap_state)
            return VX::erase(maybe_swap_state);

        auto render_target = maybe_swap_state->render_target;
        auto render_context = render_context_from(*maybe_swap_state);
        auto maybe_render_pass = instance.begin_render_pass(*render_target, render_context);
        if (!maybe_render_pass)
            return VX::erase(maybe_render_pass);

        auto graphics_context = maybe_render_pass->graphics_context();
        client.do_render(stage(), graphics_context);

        auto maybe_render_pass_end = instance.end_render_pass(maybe_render_pass.value());
        if (!maybe_render_pass_end)
            return VX::erase(maybe_render_pass_end);

        auto present_end = instance.swapchain().try_present_and_swap();
        if (!present_end)
            return VX::erase(present_end);

        return { };
    }
}
