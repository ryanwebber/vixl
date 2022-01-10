#pragma once

#include <memory>
#include <Core/Types.h>

#define VX_CREATE_SHADER(data_ptr) \
    Core::CreateShader(data_ptr, sizeof(data_ptr))

namespace Core {
    std::shared_ptr<Shader> CreateShader(const uint8_t *data, size_t length);
}
