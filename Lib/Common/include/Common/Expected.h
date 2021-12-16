#pragma once

#include <tl/expected.hpp>
#include "Error.h"

namespace Common {
    template <class T>
    using Expected = tl::expected<T, Error>;
    using Unexpected = tl::unexpected<Error>;

    template <class T, typename ...Args>
    Expected<T> MakeUnexpected(Args&& ...args) {
        Error err(args...);
        return tl::make_unexpected(std::move(err));
    }
}
