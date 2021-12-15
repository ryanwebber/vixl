#pragma once

#include <string>

namespace Core {
    class Application final {
    private:
        Application() = default;
    public:
        ~Application() = default;

        void Run();
        static void Initialize(const std::string &app_subsystem = "app");
        static Application Create() { return { }; }
    };
}
