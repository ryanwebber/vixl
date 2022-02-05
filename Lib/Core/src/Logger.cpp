#include <spdlog/sinks/stdout_color_sinks.h>
#include <VX/Core/Logger.h>

namespace VX::Core::Logger {
    std::shared_ptr<spdlog::logger> Core = create_named("core");

    std::shared_ptr<spdlog::logger> create_named(const std::string& name) {
        auto logger = spdlog::stdout_color_mt(name);
        logger->set_level(spdlog::level::debug);
        return logger;
    }
}
