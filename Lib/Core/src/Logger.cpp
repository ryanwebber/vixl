#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <Core/Logger.h>

namespace Core {
    namespace Logger {
        std::shared_ptr<spdlog::logger> Engine;
        std::shared_ptr<spdlog::logger> App;

        void Initialize(const std::string &app_name) {
            Engine = spdlog::stdout_color_mt("engine");
            Engine->set_level(spdlog::level::debug);

            App = spdlog::stdout_color_mt(app_name);
            App->set_level(spdlog::level::debug);
        }
    }
}
