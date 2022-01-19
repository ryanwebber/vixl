#pragma once

#include <string>
#include <spdlog/spdlog.h>

namespace VX::Logger {
    extern std::shared_ptr<spdlog::logger> Core;

    std::shared_ptr<spdlog::logger> Create(const std::string& name);
}
