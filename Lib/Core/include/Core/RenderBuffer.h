#pragma once

#include <memory>
#include <vector>

#include <Common/Noncopyable.h>
#include <Core/RenderCommand.h>
#include <Core/RenderBuiltins.h>

namespace Core {
    class RenderBuffer final {
        VX_MAKE_NONCOPYABLE(RenderBuffer);
    private:
        std::shared_ptr<RenderBuiltins> m_Builtins;
        std::vector<RenderCommand> m_Commands;

    public:
        explicit RenderBuffer(std::shared_ptr<RenderBuiltins> builtins)
            : m_Builtins(std::move(builtins))
            {
            }

        ~RenderBuffer() = default;

        // Move
        RenderBuffer(RenderBuffer&& other) noexcept
            : m_Builtins(std::move(other.m_Builtins))
            , m_Commands(std::move(other.m_Commands))
            {
            };

        // Move
        RenderBuffer& operator=(RenderBuffer&& other) noexcept {
            m_Commands = std::move(other.m_Commands);
            return *this;
        };

        void Clear();
        [[nodiscard]] const std::vector<RenderCommand>& GetCommands() const { return m_Commands; }

        void DrawIndexed(
                glm::mat4x4 transform,
                std::shared_ptr<VertexBufferHandle> vertex_buffer,
                std::shared_ptr<IndexBufferHandle> index_buffer,
                std::shared_ptr<Material> material
                );

        void DrawTextureQuad(
                glm::mat4x4 transform,
                std::shared_ptr<Material> material
                );
    };
}
