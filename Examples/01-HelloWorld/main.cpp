#include <VX/Core/Async.h>
#include <VX/Core/Input.h>
#include <VX/Core/Logger.h>
#include <VX/Core/Renderer.h>
#include <VX/Core/SceneManager.h>
#include <VX/Core/Window.h>
#include <VX/Entry/Main.h>
#include <VX/Platform/Platform.h>
#include <VX/Platform/Abstraction/FileSystem.h>
#include <VX/Platform/Abstraction/GraphicsInitializer.h>
#include <VX/Platform/Abstraction/WindowFactory.h>

#ifndef TARGET_FPS
#define TARGET_FPS 60
#endif

int vixl_main(const VX::Entry::Context &ctx) {

    auto logger = VX::Core::Logger::create_named("vulkan-test");

    VX::Platform::Abstraction::WindowOptions window_options = {
            .name = "Entry Test",
            .size = { 800, 600 }
    };

    auto native_window = VX::Platform::get_abstraction<VX::Platform::Abstraction::WindowFactory>()
            .create_with_options(window_options);

    auto graphics = VX::Platform::get_abstraction<VX::Platform::Abstraction::GraphicsInitializer>()
            .initialize_with_window(*native_window);

    auto resource_locator = VX::Platform::get_abstraction<VX::Platform::Abstraction::FileSystem>()
            .resource_locator();

    VX::Core::SceneManager scene_manager;
    VX::Core::EventLoop event_loop;
    VX::Core::Window window(native_window);
    VX::Core::Input input(native_window, *event_loop.executor());
    VX::Core::Renderer renderer(graphics, scene_manager.render_delegate());

    // Configure a main loop that runs at a target FPS
    static_assert(TARGET_FPS > 0, "Invalid target FPS");
    auto mills_per_frame = VX::Core::Millis(1000 / TARGET_FPS);
    auto render_timer = VX::Core::Time::Timer(event_loop.executor(), { }, mills_per_frame, true);

    // Process events every main tick
    auto process_input_handle = render_timer.subscriber().on([&](auto) {
        auto cs = input.process_events();
        if (cs == VX::Core::ControlState::Terminate) {
            event_loop.close();
        }
    });

    // Render the scene every main tick
    auto render_handle = render_timer.subscriber().on([&](auto) {
        renderer.render_frame();
    });

    event_loop.run();

    return 0;
}
