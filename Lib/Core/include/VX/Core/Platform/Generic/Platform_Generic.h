#pragma once

#include "SDL.h"
#include "SDL_syswm.h"

#include <bgfx/bgfx.h>

#include <string_view>

namespace VX::Core::Platform::Current {
    std::string_view name() {
        return "Generic";
    }

    namespace SDL {
        inline void* native_window_pointer(const SDL_SysWMinfo &wmi) {
            return nullptr;
        }
    }
}
