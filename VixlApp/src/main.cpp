#define GLAD_GL_IMPLEMENTATION 1
#include <Glad/gl.h>
#include <GLFW/glfw3.h>

#include <Common/Error.h>

#include <App/Dispatcher.h>
#include <App/GUILayer.h>
#include <App/Logger.h>
#include <App/RenderStack.h>
#include <App/WorkspaceLayer.h>
#include <App/WorkspaceRegistry.h>

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

void draw(GLFWwindow*, RenderStack&);

void OnWindowResize(GLFWwindow* window, int width, int height) {
    Dispatcher::Main().GetWindowResizeHandle().Notify({ width, height });

    // GLFW polling prevents redraws while the os window is resizing. We can
    // still do it ourselves if we perform swap buffers
    auto render_stack = static_cast<RenderStack*>(glfwGetWindowUserPointer(window));
    draw(window, *render_stack);
}

std::optional<Error> run() {
    // Init GLFW
    glfwInit();

    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Vixl " VERSION, nullptr, nullptr);

    if (window == nullptr)
    {
        glfwTerminate();
        return Error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL((GLADloadfunc) glfwGetProcAddress))
    {
        return Error("Failed to initialize OpenGL context");
    }

    // Setup window callbacks
    glfwSetWindowSizeCallback(window, OnWindowResize);

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    auto registry = std::make_shared<WorkspaceRegistry>();

    RenderStack render_stack;
    render_stack.AddLayer(std::make_unique<WorkspaceLayer>(registry));
    render_stack.AddLayer(std::make_unique<GUILayer>(window));

    glfwSetWindowUserPointer(window, static_cast<void*>(&render_stack));

    auto &dispatcher = Dispatcher::Main();
    auto ui_loop_connection = dispatcher.GetUILoopHandle().OnTimeout([&](){
        glfwPollEvents();
        draw(window, render_stack);

        if (glfwWindowShouldClose(window)) {
            dispatcher.Terminate();
        }
    });

    auto resize_connection = dispatcher.GetWindowResizeHandle().OnCallback([&](glm::vec2 size){
        render_stack.OnWindowResize(size.x, size.y);
    });

    dispatcher.GetEventLoop().Run();

    // Destroy all render layers
    render_stack.Destroy();

    // Terminates GLFW, clearing any resources allocated by GLFW.
    glfwDestroyWindow(window);
    glfwTerminate();

    return { };
}

void draw(GLFWwindow *window, RenderStack &render_stack) {
    // Clear the color buffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    render_stack.Update();
    render_stack.Render();

    // Swap the screen buffers
    glfwSwapBuffers(window);
}

int main()
{
    Logger::Initialize();
    Logger::Core->debug("Booting Vixl v{}", VERSION);

    auto maybe_error = run();
    if (maybe_error)
        Logger::Core->critical("Failed to boot application. {}", maybe_error->description());
    else
        Logger::Core->debug("Goodbye!");
}
