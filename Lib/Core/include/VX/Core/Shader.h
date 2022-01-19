#pragma once

#include <memory>
#include <span>
#include <VX/Core/Types.h>

#define VX_CREATE_SHADER(data_ptr) \
    VX::Core::CreateShader({ data_ptr, sizeof(data_ptr) })

namespace VX::Core {
    std::shared_ptr<ShaderHandle> CreateShader(std::span<const uint8_t> data);
}
