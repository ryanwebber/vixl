#include <vector>
#include <unordered_map>

#include <VX/Graphics/Instance.h>
#include <VX/Graphics/Instance_Private.h>
#include <VX/Graphics/Logger.h>

namespace VX::Graphics {

    static const std::vector<const char*> validation_layers = {
#if VX_GRAPHICS_VALIDATION
            "VK_LAYER_KHRONOS_validation"
#endif
    };

    Instance::Instance(std::unique_ptr<Private::InstanceData> instance_data)
        : m_instance_data(std::move(instance_data))
    {
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL validation_layer_callback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData)
    {
        if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
            Log::warn("[{}] {}", pCallbackData->pMessageIdName, pCallbackData->pMessage);
        }

        return VK_FALSE;
    }

    static void init_debug_messenger(const vk::raii::Instance &instance, std::vector<std::shared_ptr<void>> &resources) {
        vk::DebugUtilsMessengerCreateInfoEXT create_info = {
            .sType = vk::StructureType::eDebugUtilsMessengerCreateInfoEXT,
            .messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
            .messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
            .pfnUserCallback = validation_layer_callback,
            .pUserData = nullptr,
        };

        auto messenger = instance.createDebugUtilsMessengerEXT(create_info);
        auto shared_messenger = std::make_shared<vk::raii::DebugUtilsMessengerEXT>(std::move(messenger));
        resources.push_back(std::move(shared_messenger));
    }

    static std::vector<std::shared_ptr<void>> init_callbacks(const std::vector<const char*> &extensions, const vk::raii::Instance &instance) {
        static const std::unordered_map<const char*, std::function<void(const vk::raii::Instance&, std::vector<std::shared_ptr<void>>&)>> jump_table = {
                { VK_EXT_DEBUG_UTILS_EXTENSION_NAME , init_debug_messenger },
        };

        std::vector<std::shared_ptr<void>> resources;
        for(const auto& extension : extensions) {
            auto entry = jump_table.find(extension);
            if (entry != jump_table.end()) {
                entry->second(instance, resources);
            }
        }

        return resources;
    }

    static std::vector<const char*> init_layers(const std::vector<const char*> &requested_layers) {
        auto available_layers = vk::enumerateInstanceLayerProperties();
        Log::debug("Available validation layers ({}):", available_layers.size());
        for (const auto& discovered_layer : available_layers) {
            Log::debug("\t- {}: {}", discovered_layer.layerName, discovered_layer.description);
        }

        for (const auto requested_layer : requested_layers) {
            std::string layer_name(requested_layer);
            auto found_layer = std::find_if(available_layers.begin(), available_layers.end(), [&](const auto& layer) {
                return layer_name == layer.layerName;
            });

            if (found_layer == available_layers.end()) {
                Log::error("Requested validation layer not found: {}", layer_name);
                throw std::runtime_error("Requested validation layer not found: " + layer_name);
            }
        }

        return requested_layers;
    }

    static std::vector<const char*> init_extensions(const std::vector<const char*> &requested_extensions) {

        // Copy the requested extensions
        auto required_extensions = requested_extensions;

#if VX_GRAPHICS_VALIDATION
        required_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

        auto available_extensions = vk::enumerateInstanceExtensionProperties();
        Log::debug("Available graphics extensions ({}):", available_extensions.size());
        for (const auto extension : available_extensions) {
            Log::debug("\t- {}", extension.extensionName);
        }

        for (const auto required_extension : required_extensions) {
            std::string extension_name(required_extension);
            auto found_extension = std::find_if(available_extensions.begin(), available_extensions.end(), [&](const auto& extension) {
                return extension_name == extension.extensionName;
            });

            if (found_extension == available_extensions.end()) {
                Log::error("Required extension not found: {}", extension_name);
                throw std::runtime_error("Required extension not found: " + extension_name);
            }
        }

        return required_extensions;
    }

    std::shared_ptr<Instance> init(const GraphicsInfo &info)
    {
        init_logger();

        auto layers = init_layers(validation_layers);
        auto extensions = init_extensions(info.platform_data.required_extensions);

        vk::raii::Context context;

        vk::ApplicationInfo app_info = {
            .sType = vk::StructureType::eApplicationInfo,
            .pApplicationName = "Generic Vixl Application",
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = "Vixl",
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = VK_API_VERSION_1_2,
        };

        vk::InstanceCreateInfo instance_info = {
            .sType = vk::StructureType::eInstanceCreateInfo,
            .pApplicationInfo = &app_info,

            .enabledLayerCount = static_cast<uint32_t>(layers.size()),
            .ppEnabledLayerNames = layers.data(),

            .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
            .ppEnabledExtensionNames = extensions.data(),
        };

        vk::raii::Instance instance(context, instance_info);

        auto callbacks = init_callbacks(extensions, instance);

        auto instance_data = std::make_unique<Private::InstanceData>(
                std::move(context),
                std::move(instance),
                std::move(callbacks));

        return std::unique_ptr<Instance>(new Instance(std::move(instance_data)));
    }
}
