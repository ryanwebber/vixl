#pragma once

#include <memory>

#include <glm/glm.hpp>
#include <bgfx/bgfx.h>

#include <VX/Core/Material.h>
#include <VX/Core/Handle.h>
#include <VX/Core/Types.h>

namespace VX::Core {
    class RenderCommand final {
    private:
        RenderState m_state { BGFX_STATE_DEFAULT };
        glm::mat4x4 m_Transform { glm::mat4x4(1) };
        std::shared_ptr<VertexBufferHandle> m_VertexBuffer;
        std::shared_ptr<IndexBufferHandle> m_IndexBuffer;
        std::shared_ptr<Material> m_Material;

    public:
        RenderCommand(glm::mat4x4 transform, std::shared_ptr<VertexBufferHandle> vb, std::shared_ptr<IndexBufferHandle> ib, std::shared_ptr<Material> mat)
            : m_Transform(transform)
            , m_VertexBuffer(std::move(vb))
            , m_IndexBuffer(std::move(ib))
            , m_Material(std::move(mat))
        {}

        // Move
        RenderCommand(RenderCommand&&) noexcept = default;
        RenderCommand& operator=(RenderCommand&&) noexcept = default;

        // Copy
        RenderCommand(const RenderCommand&) noexcept = default;
        RenderCommand& operator=(const RenderCommand&) noexcept = default;

        [[nodiscard]] const RenderState& GetState() const { return m_state; }
        [[nodiscard]] const glm::mat4x4& GetModelTransform() const { return m_Transform; }
        [[nodiscard]] const VertexBufferHandle& GetVertexBuffer() const { return *m_VertexBuffer; }
        [[nodiscard]] const IndexBufferHandle& GetIndexBuffer() const { return *m_IndexBuffer; }
        [[nodiscard]] const Material& GetMaterial() const { return *m_Material; }
    };
}
