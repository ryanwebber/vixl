
// This has to come first
#define GLAD_GL_IMPLEMENTATION 1

#include <Glad/gl.h>
#include <GLFW/glfw3.h>

#include <Common/Error.h>

#include <Core/Application.h>
#include <Core/Dispatcher.h>
#include <Core/Logger.h>
#include <Core/RenderStack.h>

namespace Core {

    using Common::Error;

    // Window dimensions
    const GLuint WIDTH = 800, HEIGHT = 600;

    void draw(GLFWwindow*, RenderStack&);

    void OnWindowResize(GLFWwindow* window, int width, int height) {
        Dispatcher::Main().GetWindowResizeHandle().Notify({ .width =  width, .height = height });

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
        GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Vixl " VX_VERSION, nullptr, nullptr);
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

        auto &dispatcher = Dispatcher::Main();
        RenderStack render_stack;

        glfwSetWindowUserPointer(window, static_cast<void*>(&render_stack));

        auto ui_loop_connection = dispatcher.GetUILoopHandle().OnTimeout([&](){
            glfwPollEvents();
            draw(window, render_stack);

            if (glfwWindowShouldClose(window)) {
                dispatcher.Terminate();
            }
        });

        auto resize_connection = dispatcher.GetWindowResizeHandle().OnCallback([&](auto size) {
            Logger::Engine->debug("Window resize {}x{}", size.width, size.height);
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

    void Application::Run() {
        auto maybe_error = run();
        if (maybe_error)
            Logger::Engine->critical("Failed to boot application. {}", maybe_error->description());
        else
            Logger::Engine->debug("Goodbye!");
    }

    void Application::Initialize(const std::string &app_subsystem) {
        Logger::Initialize(app_subsystem);
        Logger::Engine->debug("Vixl v{}", VX_VERSION);
    }
}
