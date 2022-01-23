#pragma once

#include "SDL.h"
#include "SDL_syswm.h"

#include <bgfx/bgfx.h>

#include <corefoundation/CFBundle.h>
#include <climits>

#include <filesystem>
#include <string_view>

namespace VX::Core::Platform::API::OSX {
    static const std::string_view name = "OSX";

    static void initialize_platform_data(const SDL_SysWMinfo &wmi, bgfx::PlatformData &pd) {
        pd.ndt = nullptr;
        pd.nwh = wmi.info.cocoa.window;
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
