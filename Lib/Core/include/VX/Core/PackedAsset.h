#pragma once

#include <string>

namespace VX::Core {
    struct PackedAsset {
        const std::string resource_name;
        size_t offset;
        size_t size;
    };
}
