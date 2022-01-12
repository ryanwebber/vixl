#pragma once

#include <filesystem>
#include <memory>
#include <string>

#include <Common/Noncopyable.h>
#include <Common/Expected.h>

#include <Core/EventLoop.h>
#include <Core/Window.h>
#include <Core/Input.h>
#include <Core/Renderer.h>
#include <Core/RenderStack.h>
#include <Core/ResourceManager.h>
#include <Core/Size.h>

namespace Core {

    struct ApplicationSettings {
        SizeInt window_size { .width = 800, .height = 600 };
        std::filesystem::path resource_directory;
    };

    class Application final {
        VX_MAKE_NONMOVABLE(Application);
        VX_MAKE_NONCOPYABLE(Application);

    private:

        ResourceManager m_ResourceManager;

        std::shared_ptr<EventLoop> m_EventLoop;
        std::shared_ptr<Window> m_Window;
        std::shared_ptr<Renderer> m_Renderer;
        std::shared_ptr<Input> m_Input;

        explicit Application(
                const ResourceLocator&,
                std::shared_ptr<Window>,
                std::shared_ptr<Input>,
                std::shared_ptr<Renderer>,
                std::shared_ptr<EventLoop>);

    public:

        ~Application() = default;

        // Start the event loop and block until the event loop terminates
        void Run();

        // Terminate the application
        void Terminate();

        EventLoop &GetEventLoop() { return *m_EventLoop; }
        Window &GetWindow() { return *m_Window; }
        Renderer &GetRenderer() { return *m_Renderer; }
        Input &GetInput() { return *m_Input; }

        [[nodiscard]] const ResourceManager& GetResourceManager() const { return m_ResourceManager; }

        static Common::Expected<std::unique_ptr<Application>> Create(SizeInt window_size);
    };
}
