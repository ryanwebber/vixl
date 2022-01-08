#pragma once

#include <span>
#include <vector>

#include <Common/Noncopyable.h>
#include <Core/RenderCommand.h>

#define RENDER_COMMAND_CAPACITY 64

namespace Core {
    class SceneRenderer;
    class RenderTarget final {
        VX_MAKE_NONCOPYABLE(RenderTarget);
        VX_MAKE_NONMOVABLE(RenderTarget);

    private:
        bool m_Enabled = true;
        std::vector<RenderCommand> m_Commands;

        RenderTarget()
            : m_Commands(RENDER_COMMAND_CAPACITY)
        {
        }

    public:
        ~RenderTarget() = default;

        void Reset();

        std::vector<RenderCommand>& Commands() { return m_Commands; }
        [[nodiscard]] const std::vector<RenderCommand>& GetCommands() const { return m_Commands; }

        [[nodiscard]] bool IsEnabled() const { return m_Enabled; }
        void SetEnabled(bool enabled) { m_Enabled = enabled; }

        friend class SceneRenderer;
    };
}
