#pragma once

#include "SDL.h"
#include "SDL_vulkan.h"

#include <VX/Graphics/Graphics_MacOS.h>
#include <VX/Platform/Abstraction/GraphicsInitializer.h>

namespace VX::Platform::Private {
    class MacOSGraphicsInitializer: public Abstraction::GraphicsInitializer {
    private:
        MacOSGraphicsInitializer() = default;

    public:
        [[nodiscard]] std::shared_ptr<Graphics::Instance> initialize_with_window(const NativeWindow&) const override {

            // Grab the required extensions
            unsigned int extension_count = 0;
            SDL_Vulkan_GetInstanceExtensions(nullptr, &extension_count, nullptr);
            std::vector<const char*> extensionNames(extension_count);
            SDL_Vulkan_GetInstanceExtensions(nullptr, &extension_count, extensionNames.data());

            Graphics::GraphicsInfo graphics_info = {
                    .required_extensions = std::move(extensionNames)
            };

            Graphics::MacOS::PlatformData platform_data = {
            };

            return Graphics::MacOS::initialize(graphics_info, platform_data);
        }

        static const Abstraction::GraphicsInitializer& instance() {
            static MacOSGraphicsInitializer s_instance;
            return s_instance;
        }
    };
}
