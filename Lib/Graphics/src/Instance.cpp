#include <vector>

#include <VX/Graphics/Instance.h>
#include <VX/Graphics/Instance_Private.h>
#include <VX/Graphics/Logger.h>

namespace VX::Graphics {

    Instance::Instance(std::unique_ptr<Private::InstanceData> instance_data)
        : m_instance_data(std::move(instance_data))
    {
    }

    std::shared_ptr<Instance> init(const GraphicsInfo &info)
    {
        init_logger();

        std::cout << "Requested graphics extensions (" << info.platform_data.required_extensions.size() << "):" << std::endl;

        Log::debug("Requested graphics extensions ({}):", info.platform_data.required_extensions.size());
        for (const auto& extension : info.platform_data.required_extensions) {
            Log::debug("\t- {}", extension);
            std::cout << "\t- Extension: " << extension << std::endl;
        }

        vk::raii::Context context;

        vk::ApplicationInfo appInfo = {
            .sType = vk::StructureType::eApplicationInfo,
            .pApplicationName = "Generic Vixl Application",
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = "Vixl",
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = VK_API_VERSION_1_2,
        };

        vk::InstanceCreateInfo instanceInfo = {
                .sType = vk::StructureType::eInstanceCreateInfo,
                .pApplicationInfo = &appInfo,

                .enabledLayerCount = 0,
                .ppEnabledLayerNames = nullptr,

                .enabledExtensionCount = static_cast<uint32_t>(info.platform_data.required_extensions.size()),
                .ppEnabledExtensionNames = info.platform_data.required_extensions.data(),
        };

        vk::raii::Instance instance(context, instanceInfo);
        auto instance_data = std::make_unique<Private::InstanceData>(
                std::move(context),
                std::move(instance));

        return std::unique_ptr<Instance>(new Instance(std::move(instance_data)));
    }
}
