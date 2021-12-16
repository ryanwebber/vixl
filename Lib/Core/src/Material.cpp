#include <Core/Material.h>

namespace Core {

    std::shared_ptr<ShaderProgram> Compile(const VertexShader &vs, const FragmentShader &fs) {
        auto handle = bgfx::createProgram(*vs, *vs, false);
        return std::make_shared<ShaderProgram>(handle);
    }

    Material::Material(const std::string_view &name, std::shared_ptr<VertexShader> vertex_shader, std::shared_ptr<FragmentShader> fragment_shader)
        : m_Name(name)
        , m_ShaderProgram(std::move(Compile(*vertex_shader, *fragment_shader)))
        , m_VertexShader(std::move(vertex_shader))
        , m_FragmentShader(std::move(fragment_shader))
    {
    }
}
