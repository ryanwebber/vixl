#include <vector>

#include <VX/Graphics/Instance.h>
#include <VX/Graphics/Instance_Private.h>
#include <VX/Graphics/Logger.h>

namespace VX::Graphics {

    static const std::vector<const char*> validation_layers = {
#if VX_GRAPHICS_VALIDATION
            "VK_LAYER_KHRONOS_validation"
#endif
    };

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData)
    {
        if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
            std::cerr << "[WARN] " << pCallbackData->pMessage << std::endl;
            Log::warn("[{}] {}", pCallbackData->pMessageIdName, pCallbackData->pMessage);
        }

        return VK_FALSE;
    }

    Instance::Instance(std::unique_ptr<Private::InstanceData> instance_data)
        : m_instance_data(std::move(instance_data))
    {
    }

    std::shared_ptr<Instance> init(const GraphicsInfo &info)
    {
        init_logger();

        auto extensions = info.platform_data.required_extensions;

        std::cout << "Requested graphics extensions (" << extensions.size() << "):" << std::endl;
        Log::debug("Requested graphics extensions ({}):", extensions.size());
        for (const auto& extension : extensions) {
            Log::debug("\t- {}", extension);
            std::cout << "\t- " << extension << std::endl;
        }

        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
        std::cout << "Supported validation layers (" << layerCount << "):" << std::endl;
        for (const auto& layer : availableLayers) {
            Log::debug("\t- {}: {}", layer.layerName, layer.description);
            std::cout << "\t- " << layer.layerName << ": " << layer.description << std::endl;
        }

        if (layerCount == 0) {
            Log::warn("Unable to find any vulkan validation layers. Make sure the VK_LAYER_PATH environment variable is set properly.");
        }

#if VX_GRAPHICS_VALIDATION
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

        std::cout << "Using graphics extensions (" << extensions.size() << "):" << std::endl;
        Log::debug("Using graphics extensions ({}):", extensions.size());
        for (const auto& extension : extensions) {
            Log::debug("\t- {}", extension);
            std::cout << "\t- " << extension << std::endl;
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

            .enabledLayerCount = layerCount == 0 ? 0 : static_cast<uint32_t>(validation_layers.size()),
            .ppEnabledLayerNames = validation_layers.data(),

            .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
            .ppEnabledExtensionNames = extensions.data(),
        };

        vk::raii::Instance instance(context, instanceInfo);
        auto instance_data = std::make_unique<Private::InstanceData>(
                std::move(context),
                std::move(instance));

        return std::unique_ptr<Instance>(new Instance(std::move(instance_data)));
    }
}
