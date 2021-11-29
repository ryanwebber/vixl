#include <App/Logger.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Logger {
    std::shared_ptr<spdlog::logger> Core;

    void Initialize() {
        Core = spdlog::stdout_color_mt("core");
        Core->set_level(spdlog::level::debug);
    }
};
