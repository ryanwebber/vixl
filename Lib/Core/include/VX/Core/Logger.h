#pragma once

#include <string>
#include <spdlog/spdlog.h>

namespace VX::Logger {
    extern std::shared_ptr<spdlog::logger> Core;

    std::shared_ptr<spdlog::logger> create_named(const std::string& name);
}
