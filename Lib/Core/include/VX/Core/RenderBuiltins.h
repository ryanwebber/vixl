#pragma once

#include <memory>
#include <VX/Core/Types.h>

namespace VX::Core {
    struct Shape {
        std::shared_ptr<VertexBufferHandle> vertex_buffer;
        std::shared_ptr<IndexBufferHandle> index_buffer;
    };

    enum class Shapes:size_t {
        Quad = 0
    };

    class RenderBuiltins final {
    private:
        std::vector<Shape> m_shapes;

    public:
        RenderBuiltins();
        ~RenderBuiltins() = default;

        [[nodiscard]] const Shape& get_shape(Shapes shape) const {
            return m_shapes[static_cast<size_t>(shape)];
        }
    };
}
