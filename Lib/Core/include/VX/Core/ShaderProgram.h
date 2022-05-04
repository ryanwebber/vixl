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
        ShaderProgram(Graphics::SharedHandle<Graphics::HandleType::GraphicsProgram> handle, Shader vertex_shader, Shader fragment_shader)
            : m_handle(std::move(handle))
            , m_vertex_shader(std::move(vertex_shader))
            , m_fragment_shader(std::move(fragment_shader))
        {}

        ~ShaderProgram() = default;

        [[nodiscard]] const Graphics::GraphicsProgramHandle &handle() const { return *m_handle; }
    };
}
