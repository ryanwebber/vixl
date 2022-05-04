#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

#include <VX/Core/Shader.h>
#include <VX/Core/ShaderProgram.h>
#include <VX/Core/Texture.h>

namespace VX::Core {

    class Material final {
        VX_DEFAULT_MOVABLE(Material);
        VX_MAKE_NONCOPYABLE(Material);
    private:
        std::string m_name;
        ShaderProgram m_shader_program;

    public:
        Material(const std::string_view& name, ShaderProgram shader_program)
            : m_name(name)
            , m_shader_program(std::move(shader_program))
        {}

        ~Material() = default;

        [[nodiscard]] std::string_view name() const { return m_name; }
        [[nodiscard]] const ShaderProgram &shader_program() const { return m_shader_program; }
    };
}
