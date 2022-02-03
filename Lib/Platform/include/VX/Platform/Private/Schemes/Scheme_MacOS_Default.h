#pragma once

#include "SDL.h"
#include <VX/Platform/Private/SDLWindowFactory.h>

namespace VX::Platform {

    template <>
    const Abstraction::WindowFactory& get_abstraction() {
        return Private::SDLWindowFactory<SDL_WINDOW_VULKAN>::instance();
    }

    Properties properties() {
        return {
            .description = "MacOS Default (SDL,Vulkan+MoltenVK)",
        };
    }
}
