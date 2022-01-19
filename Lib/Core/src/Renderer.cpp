#include <VX/Core/Renderer.h>
#include <VX/Core/Logger.h>

#include <bgfx/bgfx.h>

namespace VX::Core {
    void Renderer::RenderFrame() {

        // This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.
        bgfx::touch(0);

        m_RenderStack.Update();
        m_RenderStack.Render();

#if VX_DEBUG_GRAPHICS
        bgfx::setDebug(BGFX_DEBUG_STATS);
#endif

        // Render the frame
        bgfx::frame();
    }

    void Renderer::Destroy() {
        m_RenderStack.Destroy();
    }
}