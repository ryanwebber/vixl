#include <VX/Core/Renderer.h>

namespace VX::Core {
    void Renderer::render_frame() {
        m_render_stack.update();
        m_render_stack.render();
    }

    void Renderer::destroy() {
        m_render_stack.destroy();
    }
}
