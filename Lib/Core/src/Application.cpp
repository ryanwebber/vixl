#include <memory>

#include <bgfx/bgfx.h>

#include <VX/Error.h>
#include <VX/Expected.h>

#include <VX/Core/Platform/Platform_Extern.h>

#include <VX/Core/Application.h>
#include <VX/Core/Logger.h>
#include <VX/Core/Renderer.h>
#include <VX/Core/Window.h>

#include <spdlog/spdlog.h>

namespace VX::Core {

    using VX::Error;
    using VX::Expected;

    namespace Test = spdlog;

    void Application::run() {

        Logger::Core->debug("Vixl v{} ({})", VX_VERSION, Platform::Current::name());

        // Run
        m_event_loop->run();

        m_renderer->destroy();

        Logger::Core->debug("Goodbye!");
    }

    void Application::terminate() {
        m_event_loop->close();
    }

    Application::Application(
            std::shared_ptr<EventLoop> event_loop,
            std::shared_ptr<Window> window,
            std::shared_ptr<Input> input,
            std::shared_ptr<Renderer> renderer)
        : m_event_loop(std::move(event_loop))
        , m_window(std::move(window))
        , m_input(std::move(input))
        , m_renderer(std::move(renderer))
    {
#define VX_ADD_EVENT_HANDLE(handle) m_event_handles.push_back(std::move(handle));

        VX_ADD_EVENT_HANDLE(m_input->on_window_resize().on([&](auto &ev) {
            Logger::Core->debug("Window resized: {}x{}", ev.window_size.width, ev.window_size.height);
            bgfx::setViewRect(0, 0, 0, ev.window_size.width, ev.window_size.height);
        }));

#undef VX_ADD_EVENT_HANDLE
    }
}
