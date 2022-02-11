#pragma once

#include <VX/Graphics/Private/Vulkan.h>

namespace VX::Graphics::Private {
    struct SwapchainSupport final {
        vk::PresentModeKHR present_mode;
        vk::SurfaceFormatKHR surface_format;
        vk::SurfaceCapabilitiesKHR surface_capabilities;
        uint32_t image_count;
    };
}
