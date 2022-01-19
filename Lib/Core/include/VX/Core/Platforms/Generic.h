#pragma once

#include <string_view>
#include <glfw/glfw.h>

namespace VX::Platforms::Generic {
    static const std::string_view name = "Generic";

    static void* GetPlatformWindowHandle(GLFWwindow* m_Window) {
        return nullptr;
    }
}
