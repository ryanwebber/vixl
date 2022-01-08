#include <entt/entity/registry.hpp>

#include <Core/Application.h>
#include <Core/Logger.h>
#include <Core/Scene.h>
#include <Core/SceneRenderer.h>
#include <Core/TimerLoopTask.h>
#include <Core/DemoRenderSystem.h>

#ifndef TARGET_FPS
    #define TARGET_FPS 60
#endif

int main()
{
    auto logger = Core::Logger::Create("myapp");
    logger->debug("Running hello world example");

    // Create an application, which opens up a native window
    auto app = Core::Application::Create({ .width = 800, .height = 600 }).value();

    // Create a new scene renderer and add it to the render stack
    auto scene_renderer = std::make_shared<Core::SceneRenderer>();
    app->GetRenderer().GetRenderStack().AddLayer(scene_renderer);

    // Create a render target for our scene to render into
    auto render_target = scene_renderer->CreateRenderTarget();

    // Create a new scene manager that will manage our various scenes
    Core::SceneManager scene_manager;

    // Create a scene and set it as the active scene
    auto scene = std::make_shared<Core::Scene>("Main");
    scene_manager.SetActiveScene(std::move(scene));

    // Configure a main loop that runs at a target FPS
    static_assert(TARGET_FPS > 0, "Invalid target FPS");
    auto mills_per_frame = Core::TimerLoopTask::Millis(1000 / TARGET_FPS);
    auto main_loop = app->GetEventLoop().Register<Core::TimerLoopTask>("Main", mills_per_frame);

    // Process events every main tick
    auto process_input_handle = main_loop->OnTimeout([&]() {
        auto cs = app->GetInput().ProcessEvents();
        if (cs == Core::ControlState::Terminate) {
            app->Terminate();
        }
    });

    // Update the scene and render a frame every main tick
    auto render_handle = main_loop->OnTimeout([&]() {
        // Update the scene and render it into our target
        scene_manager.Update();
        scene_manager.Render(*render_target);

        // Present the rendered scene onto our window
        app->GetRenderer().RenderFrame();
        app->GetWindow().GetNativeWindow().SwapBuffers();
    });

    // Start the main loop timer
    main_loop->Start();

    entt::registry registry;

    // Start the application event loop
    app->Run();
}
