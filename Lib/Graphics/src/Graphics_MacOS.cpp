#include <VX/Graphics/Graphics_MacOS.h>
#include <VX/Graphics/Private/Init.h>
#include <VX/Graphics/Private/Vulkan.h>

namespace VX::Graphics::MacOS {
    std::shared_ptr<Instance> initialize(const GraphicsInfo &graphics_info, const PlatformData &platform_data)
    {
        return Private::initialize(graphics_info, [&](vk::raii::Instance& instance) {
           return nullptr;
        });
    }
}
