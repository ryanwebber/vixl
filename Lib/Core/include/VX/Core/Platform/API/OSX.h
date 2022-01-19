#pragma once

#define GLFW_EXPOSE_NATIVE_COCOA 1
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <corefoundation/CFBundle.h>
#include <climits>

#include <filesystem>
#include <string_view>

namespace VX::Core::Platform::API::OSX {
    static const std::string_view name = "OSX";

    static void* get_platform_window_handle(GLFWwindow* m_Window) {
        return glfwGetCocoaWindow(m_Window);
    }

    static std::filesystem::path get_resource_directory() {
        CFBundleRef mainBundle = CFBundleGetMainBundle();
        CFURLRef resourceURL = CFBundleCopyResourcesDirectoryURL(mainBundle);

        char path[PATH_MAX];
        if (!CFURLGetFileSystemRepresentation(resourceURL, true, (UInt8*)path, sizeof(path))) {
            abort();
        }

        return { path };
    }
}
