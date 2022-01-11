#pragma once

#include <string_view>
#include <GLFW/glfw3.h>

#define VX_PLATFORM_OSX 1

#define GLFW_EXPOSE_NATIVE_COCOA 1
#include <GLFW/glfw3native.h>

namespace Core::Platforms::OSX {
    static const std::string_view name = "OSX";

    static void* GetPlatformWindowHandle(GLFWwindow* m_Window) {
        return glfwGetCocoaWindow(m_Window);
    }
}
