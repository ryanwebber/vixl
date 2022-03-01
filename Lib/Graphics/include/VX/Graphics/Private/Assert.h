#pragma once

#include <VX/Graphics/Private/Logger.h>

#ifndef NDEBUG
#define VX_GRAPHICS_ASSERT(Expr, Msg, ...) VX::Graphics::Private::runtime_assert(#Expr, Expr, __FILE__, __LINE__, Msg, ##__VA_ARGS__)
#else
#define VX_GRAPHICS_ASSERT(Expr, Msg) ;
#endif

namespace VX::Graphics::Private {
    template <typename ...Args>
    void runtime_assert(const char *expr_str, bool expr, const char *file, int line, spdlog::format_string_t<Args...> fmt, Args &&... args) {
        if (!expr) {
            auto message = spdlog::fmt_lib::format(fmt, std::forward<Args>(args)...);
            Log::critical("Assertion failed: {}\n\tExpected: {}\n\tAt: {}:{}", message, expr_str, file, line);
            abort();
        }
    }
}
