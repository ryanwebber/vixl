#pragma once

#include "SDL.h"
#include "SDL_syswm.h"

#include <bgfx/bgfx.h>

#include <string_view>

namespace VX::Core::Platform::API::Generic {
    static const std::string_view name = "Generic";

    static void initialize_platform_data(const SDL_SysWMinfo &wmi, bgfx::PlatformData &pd) {
        pd.ndt = nullptr;
        pd.nwh = nullptr;
    }
}
