#include <VX/Core/Renderer.h>

#include <bgfx/bgfx.h>

namespace VX::Core {
    void Renderer::render_frame() {

        // This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.
        bgfx::touch(0);

        m_render_stack.update();
        m_render_stack.render();

#if VX_DEBUG_GRAPHICS
        bgfx::setDebug(BGFX_DEBUG_STATS);
#endif

        // Render the frame
        bgfx::frame();
    }

    void Renderer::destroy() {
        m_render_stack.destroy();
    }
}
