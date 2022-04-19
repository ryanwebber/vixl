#pragma once

#include <VX/Core/Logger.h>

#ifndef NDEBUG
#define VX_CORE_ASSERT(Expr, Msg, ...) VX::Core::runtime_assert(#Expr, Expr, __FILE__, __LINE__, Msg, ##__VA_ARGS__)
#define VX_CORE_ASSERT_NOT_REACHED() VX::Core::runtime_not_reached(__FILE__, __LINE__)
#else
#define VX_CORE_ASSERT(Expr, Msg) ;
#define VX_CORE_ASSERT_NOT_REACHED() ;
#endif

namespace VX::Core {

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

    [[noreturn]] void runtime_not_reached(const char *file, int line) {
        Log::critical("Unreachable state reached at: {}:{}", file, line);
        abort();
        for (;;);
    }
}
