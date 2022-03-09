#pragma once

#include <VX/Graphics/Logger.h>

#ifndef NDEBUG
#define VX_GRAPHICS_ASSERT(Expr, Msg, ...) VX::Graphics::runtime_assert(#Expr, Expr, __FILE__, __LINE__, Msg, ##__VA_ARGS__)
#define VX_GRAPHICS_ASSERT_NOT_REACHED() VX::Graphics::runtime_assert("<not reached>", false, __FILE__, __LINE__, "Assertion not reached")
#else
#define VX_GRAPHICS_ASSERT(Expr, Msg) ;
#define VX_GRAPHICS_ASSERT_NOT_REACHED() ;
#endif

namespace VX::Graphics {

    static bool not_reached = false;

    template <typename ...Args>
    void runtime_assert(const char *expr_str, bool expr, const char *file, int line, spdlog::format_string_t<Args...> fmt, Args &&... args) {
        if (!expr) {
            auto message = spdlog::fmt_lib::format(fmt, std::forward<Args>(args)...);
            Log::critical("Assertion failed: {}\n\tExpected: {}\n\tAt: {}:{}", message, expr_str, file, line);
            abort();
            for (;;);
        }
    }
}
