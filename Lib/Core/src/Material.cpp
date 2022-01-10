#include <memory>
#include <Core/Material.h>
#include <Core/Logger.h>

namespace Core {

    std::shared_ptr<ShaderProgram> Compile(const VertexShader &vs, const FragmentShader &fs) {
        auto handle = bgfx::createProgram(*vs, *fs, false);
        return std::make_shared<ShaderProgram>(handle);
    }

    Material::Material(const std::string_view &name, std::shared_ptr<VertexShader> vertex_shader, std::shared_ptr<FragmentShader> fragment_shader)
        : m_Name(name)
        , m_ShaderProgram(std::move(Compile(*vertex_shader, *fragment_shader)))
        , m_VertexShader(std::move(vertex_shader))
        , m_FragmentShader(std::move(fragment_shader))
    {
        Logger::Core->debug("Created material '{}', vs={} fs={} program={}",
                            m_Name,
                            m_VertexShader->Get().idx,
                            m_FragmentShader->Get().idx,
                            m_ShaderProgram->Get().idx);
    }

    Material::Material(const std::string_view &name, const uint8_t *vertex_data, const size_t vertex_data_len, const uint8_t *fragment_data, const size_t fragment_data_len)
            : m_Name(name)
            , m_ShaderProgram(nullptr)
            , m_VertexShader(nullptr)
            , m_FragmentShader(nullptr)
        {
            auto vs = CreateShader(vertex_data, vertex_data_len);
            auto fs = CreateShader(fragment_data, fragment_data_len);
            m_ShaderProgram = Compile(*vs, *fs);
            m_VertexShader = std::move(vs);
            m_FragmentShader = std::move(fs);
            Logger::Core->debug("Created material '{}': vs={} fs={} program={}",
                                m_Name,
                                m_VertexShader->Get().idx,
                                m_FragmentShader->Get().idx,
                                m_ShaderProgram->Get().idx);
        }
}
