#pragma once

#include <iostream>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace VX::Core {

    constexpr const char* core_subsystem = "core";

    inline void init_logger() {
        auto logger = spdlog::stdout_color_mt(core_subsystem);
        logger->set_level(spdlog::level::debug);
    }

    namespace Log {

#if VX_CORE_DEBUG
        template<typename... Args>
        inline void debug(spdlog::format_string_t<Args...> fmt, Args &&... args) {
            spdlog::get(core_subsystem)->template debug(fmt, std::forward<Args>(args)...);
        }
#else
        template<typename... Args>
        inline void debug(spdlog::format_string_t<Args...> fmt, Args &&... args) {
        }
#endif
        template<typename... Args>
        inline void warn(spdlog::format_string_t<Args...> fmt, Args &&... args) {
            spdlog::get(core_subsystem)->template warn(fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        inline void error(spdlog::format_string_t<Args...> fmt, Args &&... args) {
            spdlog::get(core_subsystem)->template error(fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        inline void critical(spdlog::format_string_t<Args...> fmt, Args &&... args) {
            spdlog::get(core_subsystem)->template critical(fmt, std::forward<Args>(args)...);
        }
    }
}
