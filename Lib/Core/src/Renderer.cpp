#include <Core/Renderer.h>

#include <bgfx/bgfx.h>

void Core::Renderer::RenderFrame() {
    // This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.
    bgfx::touch(0);

    m_RenderStack.Update();
    m_RenderStack.Render();

    // Render the frame
    bgfx::frame();
}

void Core::Renderer::Destroy() {
    m_RenderStack.Destroy();
}
