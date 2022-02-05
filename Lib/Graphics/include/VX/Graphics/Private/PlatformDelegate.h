#pragma once

#include <memory>
#include <VX/Graphics/Private/Vulkan.h>

namespace VX::Graphics::Private {
    class PlatformDelegate {
    protected:
        PlatformDelegate() = default;
    public:
        ~PlatformDelegate() = default;

        virtual vk::raii::SurfaceKHR init_surface(vk::raii::Instance&) const = 0;
    };
}
