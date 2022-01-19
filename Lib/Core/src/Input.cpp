#include <GLFW/glfw3.h>

#include <VX/Core/Input.h>

namespace VX::Core {
    ControlState Input::process_events() {
        glfwPollEvents();

        if (glfwWindowShouldClose(m_native_window.window_pointer())) {
            return ControlState::Terminate;
        }

        return ControlState::None;
    }
}
