#include <VX/Core/RenderBuffer.h>

namespace VX::Core {
    void RenderBuffer::Clear() {
        m_Commands.clear();
    }

    void RenderBuffer::DrawIndexed(glm::mat4x4 transform,
                                   std::shared_ptr<VertexBufferHandle> vertex_buffer,
                                   std::shared_ptr<IndexBufferHandle> index_buffer,
                                   std::shared_ptr<Material> material) {
        m_Commands.emplace_back(transform,
                                std::move(vertex_buffer),
                                std::move(index_buffer),
                                std::move(material));
    }

    void RenderBuffer::DrawTextureQuad(glm::mat4x4 transform, std::shared_ptr<Material> material) {
        m_Commands.emplace_back(transform,
                                m_Builtins->GetTextureQuad().vertex_buffer,
                                m_Builtins->GetTextureQuad().index_buffer,
                                std::move(material));
    }
}
