#include <memory>
#include <VX/Core/Shader.h>

namespace VX::Core {
    std::shared_ptr<ShaderHandle> make_shader(std::span<const uint8_t> data) {
#warning "make_shader not implemented (and should be a class with reflective capabilities probably?)"
        return nullptr;
    }
}
