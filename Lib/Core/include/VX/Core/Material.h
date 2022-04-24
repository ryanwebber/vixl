#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include <VX/Copyable.h>

#include <VX/Core/Shader.h>
#include <VX/Core/ShaderProgram.h>
#include <VX/Core/Texture.h>

namespace VX::Core {

    class Material final {
        VX_DEFAULT_MOVABLE(Material);
        VX_DEFAULT_COPYABLE(Material);
    private:
        std::string m_name;
        ShaderProgram m_program;

    public:
        Material(const std::string_view& name, const ShaderProgram &program);
        ~Material() = default;
    };
}
