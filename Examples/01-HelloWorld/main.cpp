#include <Core/Application.h>
#include <Core/Logger.h>
#include <Core/TimerLoopTask.h>
#include <Core/SceneRenderer.h>
#include <Core/Scene.h>

#ifndef TARGET_FPS
    #define TARGET_FPS 60
#endif

int main()
{
    auto logger = Core::Logger::Create("myapp");
    logger->debug("Running hello world example");

    auto app = Core::Application::Create({ .width = 800, .height = 600 }).value();

    // Configure a main loop
    static_assert(TARGET_FPS > 0, "Invalid target FPS");
    auto mills_per_frame = Core::TimerLoopTask::Millis(1000 / TARGET_FPS);

    // Create a main loop that runs at 60 fps
    auto main_loop = app->GetEventLoop().Register<Core::TimerLoopTask>("Main", mills_per_frame);

    // Process events
    auto process_input_handle = main_loop->OnTimeout([&]() {
        auto cs = app->GetInput().ProcessEvents();
        if (cs == Core::ControlState::Terminate) {
            app->Terminate();
        }
    });

    // Render frame
    auto render_handle = main_loop->OnTimeout([&]() {
        app->GetRenderer().RenderFrame();
        app->GetWindow().GetNativeWindow().SwapBuffers();
    });

    // Start the main loop timer
    main_loop->Start();

    // Create a new renderer layer for the scene
    Core::SceneRenderer scene_renderer;

    // Create a new scene
    auto scene = std::make_shared<Core::Scene>();

    // Add a camera to the scene
    auto render_camera = scene_renderer.CreateRenderCamera();
    // scene->Add(render_camera);

    // Start the application event loop
    app->Run();
}
