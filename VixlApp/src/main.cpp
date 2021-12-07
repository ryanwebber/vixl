#define GLAD_GL_IMPLEMENTATION 1
#include <Glad/gl.h>
#include <GLFW/glfw3.h>

#include <Common/Error.h>

#include <App/EventLoop.h>
#include <App/GUILayer.h>
#include <App/Logger.h>
#include <App/RenderStack.h>
#include <App/TimerTask.h>
#include <App/TimerLoopTask.h>
#include <App/WorkspaceLayer.h>
#include <App/WorkspaceRegistry.h>

#ifndef TARGET_FPS
  #define TARGET_FPS 60
#endif

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

RenderStack g_RenderStack;

void draw(GLFWwindow*);

void OnWindowResize(GLFWwindow* window, int width, int height) {
    g_RenderStack.OnWindowResize(width, height);

    // GLFW polling prevents redraws while the os window is resizing. We can
    // still do it ourselves if we perform swap buffers
    draw(window);
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

    g_RenderStack.AddLayer(std::make_unique<WorkspaceLayer>(registry));
    g_RenderStack.AddLayer(std::make_unique<GUILayer>(window));

    static_assert(TARGET_FPS > 0, "Invalid target FPS");
    auto mills_per_frame = TimerTask::Millis(1000 / TARGET_FPS);

    auto loop = std::make_shared<EventLoop>();
    auto timer = loop->Register<TimerLoopTask>("Test Timer", mills_per_frame);
    auto onTimeout = timer->OnTimeout([&](){
        glfwPollEvents();
        draw(window);

        if (glfwWindowShouldClose(window)) {
            timer->Close();
        }
    });

    timer->Start();
    loop->Run();

    // Destroy all render layers
    g_RenderStack.Destroy();

    // Terminates GLFW, clearing any resources allocated by GLFW.
    glfwDestroyWindow(window);
    glfwTerminate();

    return { };
}

void draw(GLFWwindow *window) {
    // Clear the color buffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    g_RenderStack.Update();
    g_RenderStack.Render();

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
