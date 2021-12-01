#define GLAD_GL_IMPLEMENTATION 1
#include <Glad/gl.h>
#include <GLFW/glfw3.h>

#include <Common/Error.h>

#include <App/Logger.h>
#include <App/RenderStack.h>
#include <App/GUILayer.h>

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

std::optional<Error> run() {
    // Init GLFW
    glfwInit();

    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Vixl " VERSION, nullptr, nullptr);
    glfwMakeContextCurrent(window);
    if (window == nullptr)
    {
        glfwTerminate();
        return Error("Failed to create GLFW window");
    }

    if (!gladLoadGL((GLADloadfunc) glfwGetProcAddress))
    {
        return Error("Failed to initialize OpenGL context");
    }

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    RenderStack renderStack;
    renderStack.AddLayer(std::make_unique<GUILayer>(window));

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Render
        // Clear the color buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderStack.Update();
        renderStack.Render();

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    // Destroy all render layers
    renderStack.Destroy();

    // Terminates GLFW, clearing any resources allocated by GLFW.
    glfwDestroyWindow(window);
    glfwTerminate();

    return { };
}

// The MAIN function, from here we start the application and run the game loop
int main()
{
    Logger::Initialize();
    Logger::Core->debug("Booting Vixl {}", "v" VERSION);

    auto maybe_error = run();
    if (maybe_error) {
        Logger::Core->critical("Failed to boot application. {}", maybe_error->description());
    }
}