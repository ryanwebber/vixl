#include <VX/Core/RenderPass.h>

namespace VX::Core {
    void RenderPass::bind_material(const Material &material) {
        const auto& program_handle = material.shader_program().handle();
        m_graphics_context.bind_program(program_handle);
    }

    void RenderPass::draw(uint32_t vertex_count) {
        m_graphics_context.draw(vertex_count);
    }
}
