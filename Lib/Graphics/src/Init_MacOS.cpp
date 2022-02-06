#include <VX/Graphics/Init_MacOS.h>
#include <VX/Graphics/Private/Init.h>
#include <VX/Graphics/Private/PlatformDelegate.h>
#include <VX/Graphics/Private/Vulkan.h>

#ifndef VK_USE_PLATFORM_METAL_EXT
#error "Unexpected undefined VK_USE_PLATFORM_MACOS_KHR"
#endif

namespace VX::Graphics::MacOS {

    // Make sure on macOS we have all these. We literally can't do graphics if any
    // are missing so even if the user doesn't request them, we need to add them.
    static const std::vector<const char*> s_required_macos_extensions = {
            "VK_KHR_surface",
            "VK_MVK_macos_surface",
            "VK_EXT_metal_surface",
    };

    class MacOSPlatformDelegate final : public Private::PlatformDelegate {
    private:
        PlatformData m_platform_data;
    public:
        explicit MacOSPlatformDelegate(const PlatformData &platform_data)
            : m_platform_data(platform_data)
        {}

        vk::raii::SurfaceKHR init_surface(vk::raii::Instance &instance) const override {
            vk::MetalSurfaceCreateInfoEXT create_info = {
                .sType = vk::StructureType::eMetalSurfaceCreateInfoEXT,
                .pLayer = m_platform_data.metal_layer
            };

            return instance.createMetalSurfaceEXT(create_info);
        }
    };

    std::shared_ptr<Instance> initialize(const GraphicsInfo &graphics_info, const PlatformData &platform_data)
    {
        auto validated_graphics_info = graphics_info;
        for (const auto extension : s_required_macos_extensions) {
            bool found = false;
            for (const auto requested_extension : graphics_info.required_extensions) {
                if (std::strcmp(extension, requested_extension) == 0) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                validated_graphics_info.required_extensions.push_back(extension);
            }
        }

        return Private::initialize(validated_graphics_info, MacOSPlatformDelegate(platform_data));
    }
}
