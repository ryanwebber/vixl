#pragma once

#include <string_view>

namespace VX::Platform {

    struct Properties {
        const std::string_view description;
    };

    template <typename T>
    const T& get_abstraction();

    Properties properties();
}
