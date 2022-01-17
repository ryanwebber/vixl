#include <Core/Async.h>
#include <Core/Application.h>
#include <Core/DemoRenderSystem.h>
#include <Core/Logger.h>
#include <Core/Platform.h>
#include <Core/SceneManager.h>
#include <Core/Scene.h>
#include <Core/SceneRenderer.h>

#ifndef TARGET_FPS
    #define TARGET_FPS 60
#endif

int main()
{
    auto logger = Core::Logger::Create("myapp");
    logger->debug("Running hello world example");

    Core::ApplicationSettings app_settings {
        .window_size = { .width = 800, .height = 600 },
        .resource_directory = Core::Platform::GetResourceDirectory(),
    };

    // Create an application, which opens up a native window
    auto app = Core::Application::Create(app_settings).value();

    // Create a new scene renderer and add it to the render stack
    auto scene_renderer = std::make_shared<Core::SceneRenderer>();
    app->GetRenderer().GetRenderStack().AddLayer(scene_renderer);

    // Create a render context for our scene to render into
    auto render_context = scene_renderer->CreateRenderContext();

    // Create a new scene manager that will manage our various scenes
    Core::SceneManager scene_manager;

    // Create a scene and set it as the active scene
    auto scene = Core::Scene::Create("Main");
    scene->RenderSystems().push_back(std::make_shared<Core::DemoRenderSystem>());
    scene_manager.SetActiveScene(std::move(scene));

    // Configure a main loop that runs at a target FPS
    static_assert(TARGET_FPS > 0, "Invalid target FPS");
    auto mills_per_frame = Core::Async::Millis(1000 / TARGET_FPS);
    auto render_timer = Core::Async::Time::Timer(app->GetEventLoop().GetExecutor(), { }, mills_per_frame, false);

    std::filesystem::path path("/Users/rwebber/dev/hobby/vixl-cpp/build/example.txt");
    auto promise = Core::Async::FileSystem::ReadFile(app->GetEventLoop().GetExecutor(), path, 0, 64)
        .Then<Core::Empty>([&](auto result) {
            if (result.has_value()) {
                std::string contents(result.template value()->GetData(), result.template value()->GetSize());
                logger->template debug("Got data: {} (size={})", contents, result.template value()->GetSize());
            } else {
                logger->template debug("Got error: {}", result.error().what());
            }
            return Core::Empty { };
        });

    // Process events every main tick
    auto process_input_handle = render_timer.GetReceiver().On([&](auto) {
        auto cs = app->GetInput().ProcessEvents();
        if (cs == Core::ControlState::Terminate) {
            app->Terminate();
        }
    });

    // Update the scene and render a frame every main tick
    auto render_handle = render_timer.GetReceiver().On([&](auto _) {
        // Update the scene and render it into our target
        scene_manager.Update();
        scene_manager.Render(*render_context);

        // Present the rendered scene onto our window
        app->GetRenderer().RenderFrame();
        app->GetWindow().GetNativeWindow().SwapBuffers();
    });

    // Start the main loop timer
    render_timer.Start();

    // Start the application event loop
    app->Run();
}
