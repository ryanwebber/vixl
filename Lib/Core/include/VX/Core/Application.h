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

        void Run();

        // Terminate the application
        void Terminate();

        EventLoop &GetEventLoop() { return *m_EventLoop; }
        Window &GetWindow() { return *m_Window; }
        Renderer &GetRenderer() { return *m_Renderer; }
        Input &GetInput() { return *m_Input; }

        [[nodiscard]] const ResourceManager& GetResourceManager() const { return m_ResourceManager; }

        static VX::Expected<std::unique_ptr<Application>> Create(const ApplicationSettings&);
    };
}
