#include <VX/Core/Assert.h>
#include <VX/Core/RenderBuffer.h>

namespace VX::Core {
    void RenderBuffer::clear() {
        m_commands.clear();
    }

    void RenderBuffer::draw_indexed(glm::mat4x4 transform,
                                   std::shared_ptr<VertexBufferHandle> vertex_buffer,
                                   std::shared_ptr<IndexBufferHandle> index_buffer,
                                   std::shared_ptr<Material> material) {

        VX_ASSERT(vertex_buffer != nullptr && vertex_buffer->is_valid(), "Invalid vertex buffer.");
        VX_ASSERT(index_buffer != nullptr && index_buffer->is_valid(), "Invalid index buffer.");
        VX_ASSERT(material != nullptr && material->is_valid(), "Invalid material.");

        m_commands.emplace_back(transform,
                                std::move(vertex_buffer),
                                std::move(index_buffer),
                                std::move(material));
    }

    void RenderBuffer::draw_texture_quad(glm::mat4x4 transform, std::shared_ptr<Material> material) {

        VX_ASSERT(material != nullptr && material->is_valid(), "Invalid material.");

        auto quad = m_builtins->get_shape(Shapes::Quad);
        m_commands.emplace_back(transform, quad.vertex_buffer, quad.index_buffer, std::move(material));
    }
}
