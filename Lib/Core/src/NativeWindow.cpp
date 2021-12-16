#include <Core/NativeWindow.h>

#include <bgfx/platform.h>

#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

void *Core::NativeWindow::GetPlatformWindowHandle() const {
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    return (void*)(uintptr_t)glfwGetX11Window(window);
#elif BX_PLATFORM_OSX
    return glfwGetCocoaWindow(m_Window);
#elif BX_PLATFORM_WINDOWS
    return glfwGetWin32Window(window);
#else
    #error unsupported platform
#endif
}

void *Core::NativeWindow::GetPlatformDisplayType() const {
    (void)m_Window;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    return glfwGetX11Display();
#else
    return nullptr;
#endif
}

Core::SizeInt Core::NativeWindow::GetSize() const {
    SizeInt actual_window_size;
    glfwGetWindowSize(m_Window, &actual_window_size.width, &actual_window_size.height);
    return actual_window_size;
}

void Core::NativeWindow::Destroy() {
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void Core::NativeWindow::SwapBuffers() {
    glfwSwapBuffers(m_Window);
}

Common::Expected<Core::NativeWindow> Core::NativeWindow::Create(SizeInt window_size) {

    // Init GLFW
    glfwInit();

    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow(window_size.width,
                                          window_size.height,
                                          "Vixl " VX_VERSION,
                                          nullptr,
                                          nullptr);

    if (window == nullptr) {
        glfwTerminate();
        return Common::MakeUnexpected<NativeWindow>("Failed to create GLFW window");
    }

    return NativeWindow(window);
}
