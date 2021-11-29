#pragma once

#include <spdlog/spdlog.h>

namespace Logger {
    extern std::shared_ptr<spdlog::logger> Core;

    void Initialize();
};
