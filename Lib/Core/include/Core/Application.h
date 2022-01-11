#pragma once

#include <memory>
#include <string>

#include <Common/Noncopyable.h>
#include <Common/Expected.h>

#include <Core/EventLoop.h>
#include <Core/Window.h>
#include <Core/Input.h>
#include <Core/Renderer.h>
#include <Core/RenderStack.h>
#include <Core/Size.h>

namespace Core {
    class Application final {
        VX_MAKE_NONMOVABLE(Application);
        VX_MAKE_NONCOPYABLE(Application);

    private:
        std::shared_ptr<EventLoop> m_EventLoop;
        std::shared_ptr<Window> m_Window;
        std::shared_ptr<Renderer> m_Renderer;
        std::shared_ptr<Input> m_Input;

        explicit Application(std::shared_ptr<Window>, std::shared_ptr<Input>, std::shared_ptr<Renderer>, std::shared_ptr<EventLoop>);

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

        static Common::Expected<std::unique_ptr<Application>> Create(SizeInt window_size);
    };
}
