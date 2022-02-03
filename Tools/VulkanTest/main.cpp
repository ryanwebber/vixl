#include <VX/Core/Application.h>
#include <VX/Core/Async.h>
#include <VX/Core/Input.h>
#include <VX/Core/Renderer.h>
#include <VX/Core/Window.h>
#include <VX/Entry/Main.h>
#include <VX/Graphics/Instance.h>
#include <VX/Platform/Platform.h>
#include <VX/Platform/Abstraction/WindowFactory.h>

#ifndef TARGET_FPS
#define TARGET_FPS 60
#endif

int vixl_main(const VX::Entry::Context &ctx) {

    VX::Platform::Abstraction::WindowOptions window_options = {
            .name = "Entry Test",
            .size = {
                    .width = 800,
                    .height = 600,
            }
    };

    auto native_window = VX::Platform::get_abstraction<VX::Platform::Abstraction::WindowFactory>()
            .create_with_options(window_options);

    auto graphics = VX::Graphics::init({
        .platform_data = {
            .required_extensions = { }
        }
    });

    auto event_loop = std::make_shared<VX::Core::EventLoop>();
    auto window = std::make_shared<VX::Core::Window>(native_window);
    auto input = std::make_shared<VX::Core::Input>(native_window, *event_loop->executor());
    auto renderer = std::make_shared<VX::Core::Renderer>(graphics);

    VX::Core::Application app(event_loop, window, input, renderer);

    // Configure a main loop that runs at a target FPS
    static_assert(TARGET_FPS > 0, "Invalid target FPS");
    auto mills_per_frame = VX::Core::Millis(1000 / TARGET_FPS);
    auto render_timer = VX::Core::Time::Timer(app.event_loop().executor(), { }, mills_per_frame, true);

    // Process events every main tick
    auto process_input_handle = render_timer.subscriber().on([&](auto) {
        auto cs = app.input().process_events();
        if (cs == VX::Core::ControlState::Terminate) {
            app.terminate();
        }
    });

    app.run();

    return 0;
}
