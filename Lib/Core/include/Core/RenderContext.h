#pragma once

#include <memory>

#include <Common/Noncopyable.h>
#include <Core/RenderBuffer.h>
#include <Core/RenderPrimitives.h>

namespace Core {
    struct ViewProjection {
        glm::mat4x4 view_matrix { glm::mat4x4(1) };
        glm::mat4x4 projection_matrix { glm::mat4x4(1) };
    };

    class RenderContext final {
        VX_MAKE_NONCOPYABLE(RenderContext);
        VX_MAKE_NONMOVABLE(RenderContext);
    private:
        RenderBuffer m_RenderBuffer;
        ViewProjection m_ViewProjection;

    public:
        explicit RenderContext(RenderBuffer &buffer)
            : m_RenderBuffer(std::move(buffer))
            {
            }

        explicit RenderContext(std::shared_ptr<RenderPrimitives> primitives)
            : m_RenderBuffer(std::move(primitives))
            {
            }

        ~RenderContext() = default;

        void Reset();

        [[nodiscard]] const RenderBuffer& GetBuffer() const { return m_RenderBuffer; }
        RenderBuffer& Buffer() { return m_RenderBuffer; }

        [[nodiscard]] ViewProjection GetViewProjection() const { return m_ViewProjection; }
        void SetViewProjection(const ViewProjection& view_projection) { m_ViewProjection = view_projection; }
    };
}
