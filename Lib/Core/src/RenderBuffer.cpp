#include <VX/Core/RenderBuffer.h>

namespace VX::Core {
    void RenderBuffer::clear() {
        m_commands.clear();
    }

    void RenderBuffer::draw_indexed(glm::mat4x4 transform,
                                   std::shared_ptr<VertexBufferHandle> vertex_buffer,
                                   std::shared_ptr<IndexBufferHandle> index_buffer,
                                   std::shared_ptr<Material> material) {
        m_commands.emplace_back(transform,
                                std::move(vertex_buffer),
                                std::move(index_buffer),
                                std::move(material));
    }

    void RenderBuffer::draw_texture_quad(glm::mat4x4 transform, std::shared_ptr<Material> material) {
        auto quad = m_builtins->get_shape(Shapes::Quad);
        m_commands.emplace_back(transform, quad.vertex_buffer, quad.index_buffer, std::move(material));
    }
}
