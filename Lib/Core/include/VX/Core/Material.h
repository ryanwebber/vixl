#pragma once

#include <string>
#include <memory>

#include <bgfx/bgfx.h>

#include <VX/Core/Handle.h>
#include <VX/Core/Shader.h>
#include <VX/Core/Types.h>

#define VX_CREATE_MATERIAL(name, shader_name) \
    std::make_shared<VX::Core::Material>(name, std::move(VX_CREATE_SHADER(shader_name ## _vertex_shader)), std::move(VX_CREATE_SHADER(shader_name ## _fragment_shader)))

namespace VX::Core {
    class Material final {
    private:
        std::string m_name;
        std::shared_ptr<ProgramHandle> m_shader_program;
        std::shared_ptr<ShaderHandle> m_vertex_shader;
        std::shared_ptr<ShaderHandle> m_fragment_shader;

    public:
        Material(const std::string_view& name, std::shared_ptr<ShaderHandle> vertex_shader, std::shared_ptr<ShaderHandle> fragment_shader);
        Material(const std::string_view &name, const uint8_t* vertex_data, size_t vertex_data_len, const uint8_t* fragment_data, size_t fragment_data_len);
        ~Material() = default;

        [[nodiscard]] bgfx::ShaderHandle vertex_shader_handle() const { return **m_vertex_shader; }
        [[nodiscard]] bgfx::ShaderHandle fragment_shader_handle() const { return **m_fragment_shader; }
        [[nodiscard]] bgfx::ProgramHandle shader_program_handle() const { return **m_shader_program; }
    };
}
