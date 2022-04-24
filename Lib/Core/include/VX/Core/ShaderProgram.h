#pragma once

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

#include <VX/Graphics/Graphics.h>
#include <VX/Core/Shader.h>

namespace VX::Core {
    class ShaderProgram final {
        VX_DEFAULT_MOVABLE(ShaderProgram);
        VX_DEFAULT_COPYABLE(ShaderProgram);
    private:
        Graphics::SharedHandle<Graphics::HandleType::GraphicsProgram> m_handle;
        Shader m_vertex_shader;
        Shader m_fragment_shader;
    public:
        ~ShaderProgram() = default;

        [[nodiscard]] const Graphics::GraphicsProgramHandle &handle() const { return *m_handle; }
    };
}
