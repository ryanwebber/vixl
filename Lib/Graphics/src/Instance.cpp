#include <map>
#include <vector>
#include <unordered_map>

#include <VX/Graphics/Instance.h>
#include <VX/Graphics/Private/InstanceData.h>
#include <VX/Graphics/Private/Logger.h>
#include <VX/Graphics/Private/Queue.h>

namespace VX::Graphics {

    using namespace Private;

    static const std::vector<const char*> default_validation_layers = {
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

    static vk::raii::PhysicalDevice select_physical_device(const vk::raii::Instance &instance) {

        static const std::vector<std::function<int(const vk::raii::PhysicalDevice&, const vk::PhysicalDeviceProperties&)>> evaluators = {
                [](const auto& device, const auto& props) {
                    switch (props.deviceType) {
                        case vk::PhysicalDeviceType::eDiscreteGpu:
                            return 100;
                        case vk::PhysicalDeviceType::eIntegratedGpu:
                            return 50;
                        case vk::PhysicalDeviceType::eVirtualGpu:
                            return 25;
                        case vk::PhysicalDeviceType::eCpu:
                            return 10;
                        default:
                            return 0;
                    }
                },
        };

        std::multimap<int, vk::raii::PhysicalDevice> devices_rankings;
        for (auto& device : instance.enumeratePhysicalDevices()) {
            auto props = device.getProperties();
            int total_score = 0;
            for (const auto& evaluator : evaluators) {
                auto score = evaluator(device, props);
                if (score < 0) {
                    total_score = score;
                    break;
                }
            }

            if (total_score >= 0) {
                devices_rankings.insert(std::make_pair(total_score, std::move(device)));
            }
        }

        if (!devices_rankings.empty()) {
            auto &device = devices_rankings.begin()->second;
            Log::debug("Selected physical device ({} found): {}", devices_rankings.size(), device.getProperties().deviceName);
            return std::move(device);
        }

        throw std::runtime_error("No suitable graphical device found.");
    }

    static vk::raii::Device init_logical_device(const vk::raii::PhysicalDevice &physical_device, std::vector<const char*> validation_layers) {

        static const std::vector<vk::QueueFlags> required_queues = {
            // 1. A queue that supports graphics
            vk::QueueFlagBits::eGraphics,
        };

        float queue_priority = 1.0f;
        std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;

        auto queue_families = physical_device.getQueueFamilyProperties();
        for (const auto& requested_queue_flags : required_queues) {
            uint32_t i = 0;
            for (const auto& queue_family : queue_families) {
                if ((queue_family.queueFlags & requested_queue_flags) == requested_queue_flags) {

                    // Queue is valid!
                    queue_create_infos.push_back({
                        .sType = vk::StructureType::eDeviceQueueCreateInfo,
                        .pNext = nullptr,
                        .queueFamilyIndex = i,
                        .queueCount = 1,
                        .pQueuePriorities = &queue_priority,
                    });

                    break;
                }

                i++;
            }
        }

        if (queue_create_infos.size() != required_queues.size()) {
            throw std::runtime_error("One or more queue requirements could not be satisfied by the device.");
        }

        std::vector<const char*> required_extensions;
        auto supported_extensions = physical_device.enumerateDeviceExtensionProperties();
        for (const auto extension : supported_extensions) {
            // https://vulkan.lunarg.com/doc/view/1.2.198.1/mac/1.2-extensions/vkspec.html#VUID-VkDeviceCreateInfo-pProperties-04451
            if (std::string(extension.extensionName) == "VK_KHR_portability_subset") {
                required_extensions.push_back("VK_KHR_portability_subset");
            }
        }

        vk::PhysicalDeviceFeatures required_features = { /* None specified */ };
        vk::DeviceCreateInfo create_info = {
            .sType = vk::StructureType::eDeviceCreateInfo,
            .queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size()),
            .pQueueCreateInfos = queue_create_infos.data(),
            .enabledLayerCount = static_cast<uint32_t>(validation_layers.size()),
            .ppEnabledLayerNames = validation_layers.data(),
            .enabledExtensionCount = static_cast<uint32_t>(required_extensions.size()),
            .ppEnabledExtensionNames = required_extensions.data(),
            .pEnabledFeatures = &required_features,
        };

        return std::move(physical_device.createDevice(create_info));
    }

    std::shared_ptr<Instance> init(const GraphicsInfo &info)
    {
        init_logger();

        auto layers = init_layers(default_validation_layers);
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

        // Setup extension callbacks (ex. debug messaging)
        auto callbacks = init_callbacks(extensions, instance);

        // Choose a physical device to use, since we only want to use one of them
        auto physical_device = select_physical_device(instance);

        // Create a logical_device
        auto logical_device = init_logical_device(physical_device, layers);

        auto instance_data = std::make_unique<Private::InstanceData>(
                std::move(context),
                std::move(instance),
                std::move(callbacks));

        return std::unique_ptr<Instance>(new Instance(std::move(instance_data)));
    }
}
