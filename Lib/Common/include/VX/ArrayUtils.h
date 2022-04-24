#pragma once

#include <array>

namespace VX {
    template <class ArrayType>
    constexpr std::size_t Capacity() {
        return std::tuple_size<ArrayType>::value;
    }
}
