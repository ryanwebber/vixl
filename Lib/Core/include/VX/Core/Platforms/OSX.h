#pragma once

#include <filesystem>
#include <string_view>

#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_COCOA 1
#include <GLFW/glfw3native.h>

namespace VX::Platforms::OSX {
    static const std::string_view name = "OSX";

    static void* GetPlatformWindowHandle(GLFWwindow* m_Window) {
        return glfwGetCocoaWindow(m_Window);
    }

    static std::filesystem::path GetResourceDirectory();
}

#include "../src/Platforms/OSX/Foundation.inl"
