#pragma once

#include <string>
#include <memory>

#include <bgfx/bgfx.h>

#include <Core/Handle.h>
#include <Core/Shader.h>
#include <Core/Types.h>

#define VX_CREATE_MATERIAL(name, shader_name) \
    std::make_shared<Core::Material>(name, std::move(VX_CREATE_SHADER(shader_name ## _vertex_shader)), std::move(VX_CREATE_SHADER(shader_name ## _fragment_shader)))

namespace Core {
    class Material final {
    private:
        std::string m_Name;
        std::shared_ptr<ProgramHandle> m_ShaderProgram;
        std::shared_ptr<ShaderHandle> m_VertexShader;
        std::shared_ptr<ShaderHandle> m_FragmentShader;

    public:
        Material(const std::string_view& name, std::shared_ptr<ShaderHandle> vertex_shader, std::shared_ptr<ShaderHandle> fragment_shader);
        Material(const std::string_view &name, const uint8_t* vertex_data, size_t vertex_data_len, const uint8_t* fragment_data, size_t fragment_data_len);
        ~Material() = default;

        [[nodiscard]] bgfx::ShaderHandle GetVertexShaderHandle() const { return **m_VertexShader; }
        [[nodiscard]] bgfx::ShaderHandle GetFragmentShaderHandle() const { return **m_FragmentShader; }
        [[nodiscard]] bgfx::ProgramHandle GetShaderProgramHandle() const { return **m_ShaderProgram; }
    };
}
