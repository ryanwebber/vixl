#pragma once

#include <string_view>

#include <tl/expected.hpp>
#include <VX/Error.h>

namespace VX {
    template <class T>
    using Expected = tl::expected<T, Error>;
    using Unexpected = tl::unexpected<Error>;

    template <typename ...Args>
    Unexpected make_unexpected(std::string_view format, Args&& ...args) {
        auto err = VX::make_error(format, std::forward<Args>(args)...);
        return tl::make_unexpected(std::move(err));
    }

    template <typename T>
    Unexpected map_unexpected(const Expected<T> &expected) {
        auto err = expected.error();
        return tl::make_unexpected(std::move(err));
    }

    template <typename T>
    Expected<void> erase(const Expected<T> &expected) {
        return tl::make_unexpected(expected.error());
    }
}
