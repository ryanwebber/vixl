#pragma once

#include <memory>
#include <bgfx/bgfx.h>
#include <Core/Material.h>
#include <Core/Handle.h>
#include <Core/Types.h>

namespace Core {
    class RenderCommand final {
    private:
        RenderState m_state { BGFX_STATE_DEFAULT };
        std::shared_ptr<VertexBuffer> m_VertexBuffer;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
        std::shared_ptr<Material> m_Material;

    public:
        RenderCommand(std::shared_ptr<VertexBuffer> vb, std::shared_ptr<IndexBuffer> ib, std::shared_ptr<Material> mat)
            : m_VertexBuffer(std::move(vb))
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
        [[nodiscard]] const VertexBuffer& GetVertexBuffer() const { return *m_VertexBuffer; }
        [[nodiscard]] const IndexBuffer& GetIndexBuffer() const { return *m_IndexBuffer; }
        [[nodiscard]] const Material& GetMaterial() const { return *m_Material; }
    };
}
