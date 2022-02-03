#pragma once

#include <string_view>

#include "SDL.h"
#include "SDL_syswm.h"

#include <VX/Graphics/Platform.h>

namespace VX::Core::Platform::Current {
    extern std::string_view name();
    namespace SDL {
        extern void* native_window_pointer(const SDL_SysWMinfo &wmi);
    }
}
