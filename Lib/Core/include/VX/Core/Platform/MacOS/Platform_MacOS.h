#pragma once

#include "SDL.h"
#include "SDL_syswm.h"

#include <corefoundation/CFBundle.h>
#include <climits>

#include <filesystem>
#include <string_view>

namespace VX::Core::Platform::Current {

    std::string_view name() {
        return "MacOS";
    }

    namespace SDL {
        void* native_window_pointer(const SDL_SysWMinfo &wmi) {
            return wmi.info.cocoa.window;
        }
    }

    std::filesystem::path get_resource_directory() {
        CFBundleRef mainBundle = CFBundleGetMainBundle();
        CFURLRef resourceURL = CFBundleCopyResourcesDirectoryURL(mainBundle);

        char path[PATH_MAX];
        if (!CFURLGetFileSystemRepresentation(resourceURL, true, (UInt8*)path, sizeof(path))) {
            abort();
        }

        return { path };
    }
}
