#pragma once

#include <tl/expected.hpp>

#include <VX/Error.h>

namespace VX {
    template <class T>
    using Expected = tl::expected<T, Error>;
    using Unexpected = tl::unexpected<Error>;

    template <class T, typename ...Args>
    Expected<T> make_unexpected(Args&& ...args) {
        Error err(args...);
        return tl::make_unexpected(std::move(err));
    }
}
