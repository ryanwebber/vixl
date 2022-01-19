#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <VX/Core/Logger.h>

namespace VX::Logger {
    std::shared_ptr<spdlog::logger> Core = Create("core");

    std::shared_ptr<spdlog::logger> Create(const std::string& name) {
        auto logger = spdlog::stdout_color_mt(name);
        logger->set_level(spdlog::level::debug);
        return logger;
    }
}
