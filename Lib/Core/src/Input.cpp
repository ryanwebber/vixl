#include <GLFW/glfw3.h>

#include <VX/Core/Input.h>

namespace VX::Core {
    ControlState Input::ProcessEvents() {
        glfwPollEvents();

        if (glfwWindowShouldClose(m_NativeWindow.GetWindowPointer())) {
            return ControlState::Terminate;
        }

        return ControlState::None;
    }
}
