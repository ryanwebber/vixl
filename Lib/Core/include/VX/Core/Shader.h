#pragma once

#include <memory>
#include <span>
#include <VX/Core/Types.h>

#define VX_CREATE_SHADER(data_ptr) \
    VX::Core::create_shader({ data_ptr, sizeof(data_ptr) })

namespace VX::Core {
    std::shared_ptr<ShaderHandle> create_shader(std::span<const uint8_t> data);
}
