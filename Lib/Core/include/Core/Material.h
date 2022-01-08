#pragma once

#include <string>
#include <memory>

#include <bgfx/bgfx.h>

#include <Core/Handle.h>
#include <Core/Types.h>

namespace Core {
    class Material final {
    private:
        std::string m_Name;
        std::shared_ptr<VertexShader> m_VertexShader;
        std::shared_ptr<FragmentShader> m_FragmentShader;
        std::shared_ptr<ShaderProgram> m_ShaderProgram;

    public:
        Material(const std::string_view& name, std::shared_ptr<VertexShader> vertex_shader, std::shared_ptr<FragmentShader> fragment_shader);
        ~Material() = default;

        [[nodiscard]] bgfx::ShaderHandle GetVertexShaderHandle() const { return **m_VertexShader; }
        [[nodiscard]] bgfx::ShaderHandle GetFragmentShaderHandle() const { return **m_FragmentShader; }
        [[nodiscard]] bgfx::ProgramHandle GetShaderProgramHandle() const { return **m_ShaderProgram; }
    };
}
