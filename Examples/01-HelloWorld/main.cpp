#include <Core/Application.h>
#include <Core/Logger.h>
#include <Core/RenderService.h>
#include <Core/RenderSystem.h>
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

    // Create an application, which opens up a native window
    auto app = Core::Application::Create({ .width = 800, .height = 600 }).value();

    // Create a new renderer layer for scenes
    auto scene_renderer = std::make_shared<Core::SceneRenderer>();
    app->GetRenderer().GetRenderStack().AddLayer(scene_renderer);

    // Create a new test scene
    auto scene = std::make_shared<Core::Scene>();

    // Create a render service and a render camera which we'll use to render things in our scene
    auto render_service = std::make_shared<Core::RenderService>();
    render_service->AddCamera(std::move(scene_renderer->CreateRenderCamera()));

    // Create a render system which will update the render service
    Core::RenderSystem render_system(render_service);

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
        scene->Update(render_system);

        app->GetRenderer().RenderFrame();
        app->GetWindow().GetNativeWindow().SwapBuffers();
    });

    // Start the main loop timer
    main_loop->Start();

    // Start the application event loop
    app->Run();
}
