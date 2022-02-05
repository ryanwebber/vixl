#pragma once

#include <spdlog/spdlog.h>
#include <VX/Core/Logger.h>

#ifndef NDEBUG
    #define VX_ASSERT(Expr, Msg) VX::Core::runtime_assert(#Expr, Expr, __FILE__, __LINE__, Msg, *VX::Core::Logger::Core)
#else
    #define VX_ASSERT(Expr, Msg) ;
#endif

namespace VX::Core {
    void runtime_assert(const char *expr_str, bool expr, const char *file, int line, const std::string &message, spdlog::logger &logger);
}
