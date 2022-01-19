#pragma once

#include <spdlog/spdlog.h>
#include <VX/Core/Logger.h>

#ifndef NDEBUG
    #define VX_ASSERT(Expr, Msg) VX::Core::RuntimeAssert(#Expr, Expr, __FILE__, __LINE__, Msg, *VX::Logger::Core)
#else
    #define VX_ASSERT(Expr, Msg) ;
#endif

namespace VX::Core {
    void RuntimeAssert(const char *expr_str, bool expr, const char *file, int line, const char *msg, spdlog::logger &logger);
}
