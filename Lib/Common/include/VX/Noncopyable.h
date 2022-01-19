#pragma once

#define VX_MAKE_NONCOPYABLE(c) \
private:                       \
    c(const c&) = delete;      \
    c& operator=(const c&) = delete

#define VX_MAKE_NONMOVABLE(c) \
private:                      \
    c(c&&) = delete;          \
    c& operator=(c&&) = delete
