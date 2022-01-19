#include <GLFW/glfw3.h>

#include <VX/Core/NativeWindow.h>
#include <VX/Core/Platform/Platform.h>

namespace VX::Core {

    void *NativeWindow::platform_window_handle() const {
        return Platform::Current::get_platform_window_handle(m_window);
    }

    void *NativeWindow::platform_display_type() const {
        (void) m_window;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
        return glfwGetX11Display();
#else
        return nullptr;
#endif
    }

    SizeInt NativeWindow::size() const {
        SizeInt actual_window_size;
        glfwGetWindowSize(m_window, &actual_window_size.width, &actual_window_size.height);
        return actual_window_size;
    }

    void NativeWindow::destroy() {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void NativeWindow::swap_buffers() {
        glfwSwapBuffers(m_window);
    }

    VX::Expected<NativeWindow> NativeWindow::create_with_size(SizeInt window_size) {

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
            return VX::make_unexpected<NativeWindow>("Failed to create GLFW window");
        }

        return NativeWindow(window);
    }
}
