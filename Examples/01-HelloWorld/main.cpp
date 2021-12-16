#include <Core/Application.h>
#include <Core/Logger.h>
#include <Core/TimerLoopTask.h>

int main()
{
    auto logger = Core::Logger::Create("myapp");
    logger->debug("Running hello world example");
    auto app = Core::Application::Create({ .width = 800, .height = 600 }).value();

    // Configure a main loop
    auto fps = Core::TimerLoopTask::Millis(16);
    auto render_loop = app->GetEventLoop().Register<Core::TimerLoopTask>("Main", fps);
    auto _main_handle = render_loop->OnTimeout([&]() {
        auto cs = app->GetInput().ProcessEvents();
        app->GetRenderer().RenderFrame();
        app->GetWindow().GetNativeWindow().SwapBuffers();

        if (cs == Core::ControlState::Terminate) {
            app->Terminate();
        }
    });

    // Start the main loop timer
    render_loop->Start();

    // Start the application event loop
    app->Run();
}
