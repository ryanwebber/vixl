#pragma once

#include <memory>

#include <glm/glm.hpp>

#include <VX/Core/Material.h>
#include <VX/Core/Handle.h>
#include <VX/Core/Types.h>

namespace VX::Core {
    class RenderCommand final {
    private:
        glm::mat4x4 m_transform { glm::mat4x4(1) };
        std::shared_ptr<VertexBufferHandle> m_vertex_buffer;
        std::shared_ptr<IndexBufferHandle> m_index_buffer;
        std::shared_ptr<Material> m_material;

    public:
        RenderCommand(glm::mat4x4 transform, std::shared_ptr<VertexBufferHandle> vb, std::shared_ptr<IndexBufferHandle> ib, std::shared_ptr<Material> mat)
            : m_transform(transform)
            , m_vertex_buffer(std::move(vb))
            , m_index_buffer(std::move(ib))
            , m_material(std::move(mat))
        {}

        // Move
        RenderCommand(RenderCommand&&) noexcept = default;
        RenderCommand& operator=(RenderCommand&&) noexcept = default;

        // Copy
        RenderCommand(const RenderCommand&) noexcept = default;
        RenderCommand& operator=(const RenderCommand&) noexcept = default;

        [[nodiscard]] const glm::mat4x4& transform() const { return m_transform; }
        [[nodiscard]] const VertexBufferHandle& vertex_buffer() const { return *m_vertex_buffer; }
        [[nodiscard]] const IndexBufferHandle& index_buffer() const { return *m_index_buffer; }
        [[nodiscard]] const Material& material() const { return *m_material; }
    };
}
