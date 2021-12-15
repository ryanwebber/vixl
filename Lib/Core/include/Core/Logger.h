#pragma once

#include <string>
#include <spdlog/spdlog.h>

namespace Core::Logger {
    extern std::shared_ptr<spdlog::logger> Engine;
    extern std::shared_ptr<spdlog::logger> App;

    void Initialize(const std::string &app_name);
}
