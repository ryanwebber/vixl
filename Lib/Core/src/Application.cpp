#include <memory>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <VX/Error.h>
#include <VX/Expected.h>

#include <VX/Core/Graphics/Graphics.h>
#include <VX/Core/Platform/Platform.h>

#include <VX/Core/Application.h>
#include <VX/Core/Logger.h>
#include <VX/Core/Renderer.h>
#include <VX/Core/NativeWindow.h>
#include <VX/Core/Window.h>

namespace VX::Core {

    using VX::Error;
    using VX::Expected;

    // View identifier for the main window is always 0
    const bgfx::ViewId kClearView = 0;

    Expected<std::shared_ptr<Renderer>> initialize_graphics(const NativeWindow &nw) {

        SizeInt actual_window_size = nw.size();

        // Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
        bgfx::renderFrame();

        // Setup bgfx rendering
        bgfx::Init init;
        init.type = bgfx::RendererType::Metal;
        init.platformData = nw.platform_data();
        init.resolution.width = actual_window_size.width;
        init.resolution.height = actual_window_size.height;
        init.resolution.reset = BGFX_RESET_VSYNC;

        if (!bgfx::init(init)) {
            return VX::make_unexpected<std::shared_ptr<Renderer>>("Unable to initialize graphics pipeline");
        }

        // Set view 0 to the same dimensions as the window and to clear the color buffer.
        bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000ff, 1.0f, 0);
        bgfx::setViewRect(kClearView, 0, 0, actual_window_size.width, actual_window_size.height);

        Logger::Core->debug("Initial window size: {}x{}", actual_window_size.width, actual_window_size.height);

        return std::make_shared<Renderer>(nw);
    }

    void Application::run() {

        // Run
        m_event_loop->run();

        m_renderer->destroy();
        m_window->native_window().destroy();

        Logger::Core->debug("Goodbye!");
    }

    void Application::terminate() {
        m_event_loop->close();
    }

    VX::Expected<std::unique_ptr<Application>> Application::create_from_settings(const ApplicationSettings &settings) {

        Logger::Core->debug("Vixl v{}\n"
                            "  Platform: {}\n"
                            "  Graphics: {}\n"
                            "  Resource Directory: {}\n",
                            VX_VERSION,
                            Platform::Current::name,
                            Graphics::Current::name,
                            settings.resource_locator.resource_path().string());

        return NativeWindow::create_with_size(settings.window_size).and_then([&settings](NativeWindow nw) {
            return initialize_graphics(nw).map([&](std::shared_ptr<Renderer> renderer) {

                auto event_loop = std::make_shared<EventLoop>();

                auto resource_locator = settings.resource_locator;
                auto window = std::make_shared<Window>(nw);
                auto input = std::make_shared<Input>(nw, *event_loop->executor());

                auto application = new Application(resource_locator,
                                                   std::move(window),
                                                   std::move(input),
                                                   std::move(renderer),
                                                   event_loop);

                return std::unique_ptr<Application>(application);
            });
        });
    }

    Application::Application(
            const ResourceLocator& resource_locator,
            std::shared_ptr<Window> window,
            std::shared_ptr<Input> input,
            std::shared_ptr<Renderer> renderer,
            std::shared_ptr<EventLoop> event_loop)
        : m_resource_manager(event_loop->executor(), resource_locator)
        , m_window(std::move(window))
        , m_input(std::move(input))
        , m_renderer(std::move(renderer))
        , m_event_loop(std::move(event_loop))
    {
#define VX_ADD_EVENT_HANDLE(handle) m_event_handles.push_back(std::move(handle));

        VX_ADD_EVENT_HANDLE(m_input->on_window_resize().on([&](auto &ev) {
            Logger::Core->debug("Window resized: {}x{}", ev.window_size.width, ev.window_size.height);
            bgfx::setViewRect(kClearView, 0, 0, ev.window_size.width, ev.window_size.height);
        }));

#undef VX_ADD_EVENT_HANDLE
    }
}
