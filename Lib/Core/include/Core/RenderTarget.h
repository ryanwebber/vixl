#pragma once

#include <span>
#include <vector>

#include <Common/Noncopyable.h>
#include <Core/RenderCommand.h>

namespace Core {
    class SceneRenderer;
    class RenderTarget final {
        VX_MAKE_NONCOPYABLE(RenderTarget);
        VX_MAKE_NONMOVABLE(RenderTarget);

    private:
        std::vector<std::unique_ptr<RenderCommand>> m_Commands;
        bool m_Enabled = true;

        RenderTarget() = default;

    public:
        ~RenderTarget() = default;

        [[nodiscard]] std::span<const std::unique_ptr<RenderCommand>> GetRenderCommands() const { return m_Commands; }

        [[nodiscard]] bool IsEnabled() const { return m_Enabled; }
        void SetEnabled(bool enabled) { m_Enabled = enabled; }

        friend class SceneRenderer;
    };
}
