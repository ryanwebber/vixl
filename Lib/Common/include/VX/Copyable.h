#pragma once

#define VX_DEFAULT_COPYABLE(c)        \
public:                              \
    c(const c&) noexcept = default;   \
    c& operator=(const c&) noexcept = default

#define VX_DEFAULT_MOVABLE(c)    \
public:                         \
    c(c&&) noexcept = default;   \
    c& operator=(c&&) noexcept = default
