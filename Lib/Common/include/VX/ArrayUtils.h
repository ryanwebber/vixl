#pragma once

#include <array>
#include <span>

namespace VX {
    template <class ArrayType>
    constexpr std::size_t capacity() {
        return std::tuple_size<ArrayType>::value;
    }

    template <class T>
    auto as_bytes(const T& seq) {
        return std::as_bytes(std::span { seq });
    }
}
