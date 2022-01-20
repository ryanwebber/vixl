#pragma once

#include <filesystem>
#include <future>
#include <memory>
#include <string>

#include <VX/Noncopyable.h>
#include <VX/Expected.h>

#include <VX/Core/Async.h>
#include <VX/Core/Window.h>
#include <VX/Core/Input.h>
#include <VX/Core/Renderer.h>
#include <VX/Core/RenderStack.h>
#include <VX/Core/ResourceManager.h>
#include <VX/Core/Size.h>

namespace VX::Core {

    struct ApplicationSettings {
        SizeInt window_size { .width = 800, .height = 600 };
        std::filesystem::path resource_directory;
    };

    class Application final {
        VX_MAKE_NONMOVABLE(Application);
        VX_MAKE_NONCOPYABLE(Application);

    private:

        ResourceManager m_resource_manager;

        std::shared_ptr<EventLoop> m_event_loop;
        std::shared_ptr<Window> m_window;
        std::shared_ptr<Renderer> m_renderer;
        std::shared_ptr<Input> m_input;

        explicit Application(
                const ResourceLocator&,
                std::shared_ptr<Window>,
                std::shared_ptr<Input>,
                std::shared_ptr<Renderer>,
                std::shared_ptr<EventLoop>);

    public:

        ~Application() = default;

        void run();

        // Terminate the application
        void terminate();

        EventLoop &event_loop() { return *m_event_loop; }
        Window &window() { return *m_window; }
        Renderer &renderer() { return *m_renderer; }
        Input &input() { return *m_input; }

        [[nodiscard]] const ResourceManager& resource_manager() const { return m_resource_manager; }

        static VX::Expected<std::unique_ptr<Application>> create_from_settings(const ApplicationSettings&);
    };
}