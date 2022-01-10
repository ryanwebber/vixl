#pragma once

#include <vector>
#include <Common/Noncopyable.h>
#include <Core/RenderCommand.h>

namespace Core {
    class RenderBuffer final {
        VX_MAKE_NONCOPYABLE(RenderBuffer);
    private:
        std::vector<RenderCommand> m_Commands;
    public:
        RenderBuffer() = default;
        ~RenderBuffer() = default;

        // Move
        RenderBuffer(RenderBuffer&& other) noexcept
            : m_Commands(std::move(other.m_Commands))
            {
            };

        // Move
        RenderBuffer& operator=(RenderBuffer&& other) noexcept {
            m_Commands = std::move(other.m_Commands);
            return *this;
        };

        void Clear();
        [[nodiscard]] const std::vector<RenderCommand>& GetCommands() const { return m_Commands; }

        void DrawIndexed(glm::mat4x4 transform,
                         std::shared_ptr<VertexBuffer> vertex_buffer,
                         std::shared_ptr<IndexBuffer> index_buffer,
                         std::shared_ptr<Material> material);
    };
}
