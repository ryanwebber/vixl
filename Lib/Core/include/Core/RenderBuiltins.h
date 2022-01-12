#pragma once

#include <memory>
#include <Core/Types.h>

namespace Core {
    struct Shape {
        std::shared_ptr<VertexBufferHandle> vertex_buffer;
        std::shared_ptr<IndexBufferHandle> index_buffer;
    };

    class RenderBuiltins final {
    private:
        Shape m_TextureQuad;

    public:
        RenderBuiltins();
        ~RenderBuiltins() = default;

        [[nodiscard]] const Shape& GetTextureQuad() const { return m_TextureQuad; }
    };
}
