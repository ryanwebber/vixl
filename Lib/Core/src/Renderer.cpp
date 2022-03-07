#include <VX/Core/Renderer.h>

namespace VX::Core {
    void Renderer::render_frame() {
        m_render_stack.update();

        auto& swapchain = m_graphics->swapchain();
        auto state = swapchain.frame_sequencer().acquire_next_swap_state().value();
        auto render_pass = m_graphics->render_pipeline().try_begin_render_pass(state.create_render_request()).value();

        m_render_stack.render();

        // TODO: Remove this
        render_pass.command_recorder().bind(m_graphics->example_graphics_pipeline());
        render_pass.command_recorder().draw();

        m_graphics->render_pipeline().end_render_pass(render_pass);

        swapchain.frame_synchronizer().swap_and_present(state);
    }

    void Renderer::destroy() {
        m_render_stack.destroy();
    }
}
