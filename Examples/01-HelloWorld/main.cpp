#include <VX/Core/Async.h>
#include <VX/Core/Application.h>
#include <VX/Core/DemoRenderSystem.h>
#include <VX/Core/Logger.h>
#include <VX/Core/Platform.h>
#include <VX/Core/SceneManager.h>
#include <VX/Core/Scene.h>
#include <VX/Core/SceneRenderer.h>

#ifndef TARGET_FPS
    #define TARGET_FPS 60
#endif

int main()
{
    auto logger = VX::Logger::Create("myapp");
    logger->debug("Running hello world example");

    VX::Core::EventLoop::RunScoped([&](auto executor) -> std::vector<VX::Core::Closable> {
        auto promise = VX::Core::FileSystem::ReadFile(*executor, "/Users/rwebber/dev/hobby/vixl-cpp/build/example.txt", 0, 64)
                .Finally([&](auto& result) {
                    if (result.has_value()) {
                        auto span = result.value().GetView();
                        std::string contents((char*)span.data(), span.size());
                        logger->debug("Got data: {} (size={})", contents, span.size());
                    } else {
                        logger->debug("Got error: {}", result.error().what());
                    }
                });

        std::vector<VX::Core::Closable> handles;
        handles.push_back(std::move(promise));
        return handles;
    });

    VX::Core::ApplicationSettings app_settings {
        .window_size = { .width = 800, .height = 600 },
        .resource_directory = VX::Core::Platform::GetResourceDirectory(),
    };

    // Create an application, which opens up a native window
    auto app = VX::Core::Application::Create(app_settings).value();

    // Create a new scene renderer and add it to the render stack
    auto scene_renderer = std::make_shared<VX::Core::SceneRenderer>();
    app->GetRenderer().GetRenderStack().AddLayer(scene_renderer);

    // Create a render context for our scene to render into
    auto render_context = scene_renderer->CreateRenderContext();

    // Create a new scene manager that will manage our various scenes
    VX::Core::SceneManager scene_manager;

    // Create a scene and set it as the active scene
    auto scene = VX::Core::Scene::Create("Main");
    scene->RenderSystems().push_back(std::make_shared<VX::Core::DemoRenderSystem>());
    scene_manager.SetActiveScene(std::move(scene));

    // Configure a main loop that runs at a target FPS
    static_assert(TARGET_FPS > 0, "Invalid target FPS");
    auto mills_per_frame = VX::Core::Millis(1000 / TARGET_FPS);
    auto render_timer = VX::Core::Time::Timer(app->GetEventLoop().GetExecutor(), { }, mills_per_frame, false);

    // Process events every main tick
    auto process_input_handle = render_timer.GetReceiver().On([&](auto) {
        auto cs = app->GetInput().ProcessEvents();
        if (cs == VX::Core::ControlState::Terminate) {
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
