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
}
