#include <memory>
#include <bgfx/bgfx.h>
#include <Core/Shader.h>

namespace Core {
    std::shared_ptr<ShaderHandle> CreateShader(std::span<const uint8_t> data) {
        auto memory = bgfx::makeRef(data.data(), data.size(), nullptr, nullptr);
        auto handle = bgfx::createShader(memory);
        return std::make_shared<ShaderHandle>(handle);
    }
}
