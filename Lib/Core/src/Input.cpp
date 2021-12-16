#include <GLFW/glfw3.h>

#include <Core/Input.h>

namespace Core {
    ControlState Core::Input::ProcessEvents() {
        glfwPollEvents();

        if (glfwWindowShouldClose(m_NativeWindow.GetWindowPointer())) {
            return ControlState::Terminate;
        }

        return ControlState::None;
    }
}
