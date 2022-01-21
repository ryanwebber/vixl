#include <memory>
#include <bgfx/bgfx.h>
#include <VX/Core/Shader.h>

namespace VX::Core {
    std::shared_ptr<ShaderHandle> make_shader(std::span<const uint8_t> data) {
        auto memory = bgfx::makeRef(data.data(), data.size(), nullptr, nullptr);
        auto handle = bgfx::createShader(memory);
        return std::make_shared<ShaderHandle>(handle);
    }
}
