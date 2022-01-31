#pragma once

#include <iostream>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace VX::Graphics {

    inline void init_logger() {
        auto logger = spdlog::stdout_color_mt("Graphics");
        logger->set_level(spdlog::level::debug);
        spdlog::set_default_logger(logger);
    }

#if VX_GRAPHICS_DEBUG
    namespace Log = spdlog;
#else
    namespace Log {
        template<typename... Args>
        inline void debug(spdlog::format_string_t<Args...> fmt, Args &&... args) {}

        template<typename... Args>
        inline void warn(spdlog::format_string_t<Args...> fmt, Args &&... args) {}

        template<typename... Args>
        inline void error(spdlog::format_string_t<Args...> fmt, Args &&... args) {}
    }
#endif
}
