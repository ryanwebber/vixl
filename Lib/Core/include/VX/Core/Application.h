#pragma once

#include <filesystem>
#include <future>
#include <memory>
#include <string>
#include <vector>

#include <VX/Noncopyable.h>
#include <VX/Expected.h>

#include <VX/Core/Async.h>
#include <VX/Core/Closable.h>
#include <VX/Core/Input.h>
#include <VX/Core/RenderStack.h>
#include <VX/Core/Renderer.h>
#include <VX/Core/ResourceLocator.h>
#include <VX/Core/ResourceManager.h>
#include <VX/Core/Size.h>
#include <VX/Core/Window.h>

namespace VX::Core {

    class Application final {
        VX_MAKE_NONMOVABLE(Application);
        VX_MAKE_NONCOPYABLE(Application);

    private:
        std::shared_ptr<EventLoop> m_event_loop;
        std::shared_ptr<Window> m_window;
        std::shared_ptr<Input> m_input;
        std::shared_ptr<Renderer> m_renderer;
        std::vector<Closable> m_event_handles;

    public:
        Application(
                std::shared_ptr<EventLoop> event_loop,
                std::shared_ptr<Window> window,
                std::shared_ptr<Input> input,
                std::shared_ptr<Renderer> renderer);

        ~Application() = default;

        void run();

        // Terminate the application
        void terminate();

        EventLoop &event_loop() { return *m_event_loop; }
        Window &window() { return *m_window; }
        Renderer &renderer() { return *m_renderer; }
        Input &input() { return *m_input; }
    };
}
