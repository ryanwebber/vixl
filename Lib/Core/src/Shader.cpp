#include <memory>
#include <bgfx/bgfx.h>
#include <Core/Shader.h>

namespace Core {
    std::shared_ptr<Shader> CreateShader(const uint8_t *data, size_t length) {
        auto memory = bgfx::makeRef(data, length, nullptr, nullptr);
        auto handle = bgfx::createShader(memory);
        return std::make_shared<Shader>(handle);
    }
}
