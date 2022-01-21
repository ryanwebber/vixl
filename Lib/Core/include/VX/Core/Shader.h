#pragma once

#include <memory>
#include <span>
#include <VX/Core/Types.h>

namespace VX::Core {
    std::shared_ptr<ShaderHandle> make_shader(std::span<const uint8_t> data);
}
