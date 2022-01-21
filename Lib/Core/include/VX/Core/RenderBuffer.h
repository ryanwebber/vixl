#pragma once

#include <memory>
#include <vector>

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>
#include <VX/Core/RenderCommand.h>
#include <VX/Core/RenderBuiltins.h>

namespace VX::Core {
    class RenderBuffer final {
        VX_MAKE_NONCOPYABLE(RenderBuffer);
        VX_DEFAULT_MOVABLE(RenderBuffer);
    private:
        std::shared_ptr<RenderBuiltins> m_builtins;
        std::vector<RenderCommand> m_commands;

    public:
        explicit RenderBuffer(std::shared_ptr<RenderBuiltins> builtins)
            : m_builtins(std::move(builtins))
            {
            }

        ~RenderBuffer() = default;

        void clear();

        [[nodiscard]] const RenderBuiltins &builtins() const { return *m_builtins; }
        [[nodiscard]] const std::vector<RenderCommand>& commands() const { return m_commands; }

        void draw_indexed(
                glm::mat4x4 transform,
                std::shared_ptr<VertexBufferHandle> vertex_buffer,
                std::shared_ptr<IndexBufferHandle> index_buffer,
                std::shared_ptr<Material> material
                );

        void draw_texture_quad(
                glm::mat4x4 transform,
                std::shared_ptr<Material> material
                );
    };
}
