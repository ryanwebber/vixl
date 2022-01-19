#include <memory>

#include <GLFW/glfw3.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <VX/Error.h>
#include <VX/Expected.h>

#include <VX/Core/Graphics.h>
#include <VX/Core/Platform.h>

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

    void OnWindowResize(GLFWwindow* window, int width, int height) {
        // GLFW polling prevents redraws while the os window is resizing. We can
        // still do it ourselves if we perform swap buffers
        bgfx::reset((uint32_t)width, (uint32_t)height, BGFX_RESET_VSYNC);
        bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
    }

    Expected<std::shared_ptr<Renderer>> InitializeGraphics(const NativeWindow &nw) {

        SizeInt actual_window_size = nw.GetSize();

        // Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
        bgfx::renderFrame();

        // Setup bgfx rendering
        bgfx::Init init;
        init.type = bgfx::RendererType::Metal;
        init.platformData.nwh = nw.GetPlatformWindowHandle();
        init.platformData.ndt = nw.GetPlatformDisplayType();
        init.resolution.width = actual_window_size.width;
        init.resolution.height = actual_window_size.height;
        init.resolution.reset = BGFX_RESET_VSYNC;

        if (!bgfx::init(init)) {
            return VX::MakeUnexpected<std::shared_ptr<Renderer>>("Unable to initialize graphics pipeline");
        }

        // Set view 0 to the same dimensions as the window and to clear the color buffer.
        bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000ff, 1.0f, 0);
        bgfx::setViewRect(kClearView, 0, 0, actual_window_size.width, actual_window_size.height);

        Logger::Core->debug("Initial window size: {}x{}", actual_window_size.width, actual_window_size.height);

        return std::make_shared<Renderer>();
    }

    void Application::Run() {

        // Run
        m_EventLoop->Run();

        m_Renderer->Destroy();
        m_Window->GetNativeWindow().Destroy();

        Logger::Core->debug("Goodbye!");
    }

    void Application::Terminate() {
        m_EventLoop->Close();
    }

    VX::Expected<std::unique_ptr<Application>> Application::Create(const ApplicationSettings &settings) {

        Logger::Core->debug("Vixl v{}\n"
                            "  Platform: {}\n"
                            "  Graphics: {}\n"
                            "  Resource Directory: {}\n",
                            VX_VERSION,
                            Platform::name,
                            GraphicsAPI::name,
                            settings.resource_directory.string());

        return NativeWindow::Create(settings.window_size).and_then([&settings](NativeWindow nw) {
            return InitializeGraphics(nw).map([&](std::shared_ptr<Renderer> renderer) {

                ResourceLocator resource_locator(settings.resource_directory);

                auto window = std::make_shared<Window>(nw);
                auto input = std::make_shared<Input>(nw);
                auto event_loop = std::make_shared<EventLoop>();

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
        : m_ResourceManager(resource_locator, event_loop)
        , m_Window(std::move(window))
        , m_Input(std::move(input))
        , m_Renderer(std::move(renderer))
        , m_EventLoop(std::move(event_loop))
    {
        // Hook into GLFW to get involved in events. First, we set this application to
        // be the userdata pointer, so this instance can handle the events
        glfwSetWindowUserPointer(m_Window->GetNativeWindow().GetWindowPointer(), static_cast<void*>(this));

        // Setup window callbacks
        glfwSetWindowSizeCallback(m_Window->GetNativeWindow().GetWindowPointer(), OnWindowResize);
    }
}
