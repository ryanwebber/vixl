#pragma once

#include "SDL.h"
#include "SDL_syswm.h"
#include "SDL_vulkan.h"

#include <VX/Graphics/Init_MacOS.h>
#include <VX/Platform/Abstraction/GraphicsInitializer.h>

namespace VX::Platform::Private {
    class MacOSGraphicsInitializer: public Abstraction::GraphicsInitializer {
    private:
        MacOSGraphicsInitializer() = default;

    public:
        [[nodiscard]] std::shared_ptr<Graphics::Instance> initialize_with_window(NativeWindow &native_window) const override {

            // Grab the required extensions
            unsigned int extension_count = 0;
            SDL_Vulkan_GetInstanceExtensions(nullptr, &extension_count, nullptr);
            std::vector<const char*> extensionNames(extension_count);
            SDL_Vulkan_GetInstanceExtensions(nullptr, &extension_count, extensionNames.data());

            auto renderer = native_window.create_renderer();
            auto metal_layer = SDL_RenderGetMetalLayer(renderer);

            glm::vec<2, int> framebuffer_dimensions = { };
            SDL_GetRendererOutputSize(renderer, &framebuffer_dimensions.x, &framebuffer_dimensions.y);

            Graphics::GraphicsInfo graphics_info = {
                    .required_extensions = std::move(extensionNames),
                    .framebuffer_dimensions = framebuffer_dimensions,
            };

            Graphics::MacOS::PlatformData platform_data = {
                    .metal_layer = static_cast<Graphics::MacOS::CAMetalLayer*>(metal_layer),
            };

            return Graphics::MacOS::initialize(graphics_info, platform_data);
        }

        static const Abstraction::GraphicsInitializer& instance() {
            static MacOSGraphicsInitializer s_instance;
            return s_instance;
        }
    };
}
