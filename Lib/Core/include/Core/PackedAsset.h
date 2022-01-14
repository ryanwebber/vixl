#pragma once

#include <string>

namespace Core {
    struct PackedAsset final {
        const std::string resource_name;
        size_t offset;
        size_t size;
    };
}
