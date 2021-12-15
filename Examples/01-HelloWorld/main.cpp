#include <Core/Application.h>
#include <Core/Logger.h>

int main()
{
    Core::Application::Initialize("myapp");
    Core::Logger::App->debug("Running hello world example");

    auto app = Core::Application::Create();
    app.Run();
}
