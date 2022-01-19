#pragma once

#include <memory>

#include <VX/Noncopyable.h>
#include <VX/Core/RenderBuffer.h>
#include <VX/Core/RenderBuiltins.h>

namespace VX::Core {
    struct ViewProjection {
        glm::mat4x4 view_matrix { glm::mat4x4(1) };
        glm::mat4x4 projection_matrix { glm::mat4x4(1) };
    };

    class RenderContext final {
        VX_MAKE_NONCOPYABLE(RenderContext);
        VX_MAKE_NONMOVABLE(RenderContext);
    private:
        RenderBuffer m_render_buffer;
        ViewProjection m_view_projection;

    public:
        explicit RenderContext(RenderBuffer &buffer)
            : m_render_buffer(std::move(buffer))
            {
            }

        explicit RenderContext(std::shared_ptr<RenderBuiltins> builtins)
            : m_render_buffer(std::move(builtins))
            {
            }

        ~RenderContext() = default;

        void reset();

        [[nodiscard]] const RenderBuffer& buffer() const { return m_render_buffer; }
        RenderBuffer& buffer() { return m_render_buffer; }

        [[nodiscard]] ViewProjection view_projection() const { return m_view_projection; }
        void set_view_projection(const ViewProjection& view_projection) { m_view_projection = view_projection; }
    };
}
