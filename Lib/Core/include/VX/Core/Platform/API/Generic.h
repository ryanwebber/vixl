#pragma once

#include <string_view>
#include <glfw/glfw.h>

namespace VX::Core::Platform::API::Generic {
    static const std::string_view name = "Generic";

    static void* get_platform_window_handle(GLFWwindow* m_Window) {
        return nullptr;
    }
}
