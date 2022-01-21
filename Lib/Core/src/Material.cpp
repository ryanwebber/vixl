#include <memory>

#include <VX/Core/Logger.h>
#include <VX/Core/Material.h>
#include <VX/Core/Shader.h>

namespace VX::Core {

    std::shared_ptr<ProgramHandle> compile(const ShaderHandle &vs, const ShaderHandle &fs) {
        auto handle = bgfx::createProgram(*vs, *fs, false);
        return std::make_shared<ProgramHandle>(handle);
    }

    Material::Material(const std::string_view &name, std::shared_ptr<ShaderHandle> vertex_shader, std::shared_ptr<ShaderHandle> fragment_shader)
        : m_name(name)
        , m_shader_program(std::move(compile(*vertex_shader, *fragment_shader)))
        , m_vertex_shader(std::move(vertex_shader))
        , m_fragment_shader(std::move(fragment_shader))
    {
        Logger::Core->debug("Created material '{}', vs={} fs={} program={}",
                            m_name,
                            m_vertex_shader->get().idx,
                            m_fragment_shader->get().idx,
                            m_shader_program->get().idx);
    }

    Material::Material(const std::string_view &name, const uint8_t *vertex_data, const size_t vertex_data_len, const uint8_t *fragment_data, const size_t fragment_data_len)
            : m_name(name)
            , m_shader_program(nullptr)
            , m_vertex_shader(nullptr)
            , m_fragment_shader(nullptr)
        {
            auto vs = make_shader({ vertex_data, vertex_data_len });
            auto fs = make_shader({ fragment_data, fragment_data_len });
            m_shader_program = compile(*vs, *fs);
            m_vertex_shader = std::move(vs);
            m_fragment_shader = std::move(fs);
            Logger::Core->debug("Created material '{}': vs={} fs={} program={}",
                                m_name,
                                m_vertex_shader->get().idx,
                                m_fragment_shader->get().idx,
                                m_shader_program->get().idx);
        }

    Material Material::clone() const {
        return { *this };
    }
}
