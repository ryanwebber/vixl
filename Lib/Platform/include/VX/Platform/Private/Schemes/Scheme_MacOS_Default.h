#pragma once

#include "SDL.h"

#include <VX/Platform/Private/MacOSBundleFileSystem.h>
#include <VX/Platform/Private/MacOSGraphicsInitializer.h>
#include <VX/Platform/Private/SDLWindowFactory.h>

namespace VX::Platform {

    template <>
    const Abstraction::WindowFactory& get_abstraction() {
        return Private::SDLWindowFactory<SDL_WINDOW_VULKAN>::instance();
    }

    template <>
    const Abstraction::GraphicsInitializer& get_abstraction() {
        return Private::MacOSGraphicsInitializer::instance();
    }

    template <>
    const Abstraction::FileSystem& get_abstraction() {
        return Private::MacOSBundleFileSystem::instance();
    }

    Properties properties() {
        return {
            .description = "MacOS Default (SDL,Vulkan+MoltenVK)",
        };
    }
}
