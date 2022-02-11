#include <map>
#include <vector>
#include <unordered_map>

#include <VX/Graphics/Instance.h>
#include <VX/Graphics/Private/Init.h>
#include <VX/Graphics/Private/InstanceData.h>
#include <VX/Graphics/Private/Logger.h>
#include <VX/Graphics/Private/QueueSupport.h>
#include <VX/Graphics/Private/SwapchainSupport.h>
#include <VX/Graphics/Private/Vulkan.h>

namespace VX::Graphics::Private {

    static const std::vector<const char*> default_validation_layers = {
#if VX_GRAPHICS_VALIDATION
            "VK_LAYER_KHRONOS_validation"
#endif
    };

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
        Log::debug("Requested validation layers ({}):", requested_layers.size());
        for (const auto requested_layer : requested_layers) {
            std::string layer_name(requested_layer);
            auto found_layer = std::find_if(available_layers.begin(), available_layers.end(), [&](const auto& layer) {
                return layer_name == layer.layerName;
            });

            if (found_layer == available_layers.end()) {
                Log::error("Requested validation layer not found: {}", layer_name);
                throw std::runtime_error("Requested validation layer not found: " + layer_name);
            }

            Log::debug("\t- {}: {}", found_layer->layerName, found_layer->description);
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
        Log::debug("Requested graphics extensions ({}):", requested_extensions.size());
        for (const auto required_extension : required_extensions) {
            Log::debug("\t- {}", required_extension);
            std::string extension_name(required_extension);
            auto found_extension = std::find_if(available_extensions.begin(), available_extensions.end(), [&](const auto& extension) {
                return extension_name == extension.extensionName;
            });

            if (found_extension == available_extensions.end()) {
                Log::error("Required extension not found: {}. Available extensions:", extension_name);
                for (const auto& available_extension : available_extensions) {
                    Log::debug("\t- {}", available_extension.extensionName);
                }

                throw std::runtime_error("Required extension not found: " + extension_name);
            }
        }

        return required_extensions;
    }

    static vk::raii::PhysicalDevice select_physical_device(const vk::raii::Instance &instance, const vk::raii::SurfaceKHR &surface) {

        static const std::vector<std::function<int(const vk::raii::PhysicalDevice&)>> evaluators = {
                [&](const auto& device) {
                    // Ensure the device supports the render surface
                    for (auto i = 0; i < device.getQueueFamilyProperties().size(); i++) {
                        if (device.getSurfaceSupportKHR(i, *surface)) {
                            return 0;
                        }
                    }

                    return -1;
                },
                [](const auto& device) {
                    // Order the devices based on the type
                    switch (device.getProperties().deviceType) {
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
            int total_score = 0;
            for (const auto& evaluator : evaluators) {
                auto score = evaluator(device);
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

    static QueueSupport query_queue_support(const vk::raii::PhysicalDevice &physical_device, const vk::raii::SurfaceKHR &surface)
    {
        QueueSupportBuilder queue_builder({
            QueueFeature::Graphics,
            QueueFeature::Presentation,
        });

        uint32_t i = 0;
        for (const auto &queue_family_props: physical_device.getQueueFamilyProperties()) {

            if (queue_family_props.queueFlags & vk::QueueFlagBits::eGraphics) {
                queue_builder.add_feature_for_index(i, QueueFeature::Graphics);
            }

            if (physical_device.getSurfaceSupportKHR(i, *surface)) {
                queue_builder.add_feature_for_index(i, QueueFeature::Presentation);
            }

            // Break out early if we can
            if (queue_builder.is_fully_satisfied())
                break;

            i++;
        }

        if (!queue_builder.is_fully_satisfied()) {
            throw std::runtime_error("Unable to satisfy one or more device queue requirements with the selected device");
        }

        return queue_builder.build();
    }

    static vk::raii::Device init_logical_device(
                const vk::raii::PhysicalDevice &physical_device,
                const QueueSupport &queue_support,
                std::vector<const char*> validation_layers) {

        std::vector<const char *> required_extensions = {
            // Add the swapchain extension, because we'll need to create a swapchain...
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        };

        auto supported_extensions = physical_device.enumerateDeviceExtensionProperties();
        for (const auto extension: supported_extensions) {
            // https://vulkan.lunarg.com/doc/view/1.2.198.1/mac/1.2-extensions/vkspec.html#VUID-VkDeviceCreateInfo-pProperties-04451
            if (std::string(extension.extensionName) == "VK_KHR_portability_subset") {
                required_extensions.push_back("VK_KHR_portability_subset");
            }
        }

        float queue_priority = 1.0f;
        std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;
        for(const auto idx : queue_support.unique_queues()) {
            queue_create_infos.push_back({
                 .sType = vk::StructureType::eDeviceQueueCreateInfo,
                 .queueFamilyIndex = idx,
                 .queueCount = 1,
                 .pQueuePriorities = &queue_priority,
            });
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

    static SwapchainSupport query_swapchain_support(const vk::raii::PhysicalDevice &physical_device, const vk::raii::SurfaceKHR &surface) {
        auto capabilities = physical_device.getSurfaceCapabilitiesKHR(*surface);
        auto color_formats = physical_device.getSurfaceFormatsKHR(*surface);
        auto present_modes = physical_device.getSurfacePresentModesKHR(*surface);

        auto preferred_color_format = vk::Format::eB8G8R8A8Srgb;
        auto selected_color_format_itr = std::find_if(color_formats.begin(), color_formats.end(), [&](const auto& color_format) {
            return color_format.format == preferred_color_format
                   && color_format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear;
        });

        if (selected_color_format_itr == color_formats.end())
            throw std::runtime_error("No supported color format found");

        auto selected_presentation_mode = vk::PresentModeKHR::eMailbox;
        auto selected_presentation_mode_itr = std::find(present_modes.begin(), present_modes.end(), selected_presentation_mode);
        if (selected_presentation_mode_itr == present_modes.end())
            selected_presentation_mode = vk::PresentModeKHR::eFifo;

        // Recommended use at least the min value plus one, so we don't have to
        // wait on the graphics driver
        auto image_count = std::max(capabilities.minImageCount + 1, capabilities.maxImageCount);

        return {
            .present_mode = selected_presentation_mode,
            .surface_format = *selected_color_format_itr,
            .surface_capabilities = capabilities,
            .image_count = image_count,
        };
    }

    static vk::raii::SwapchainKHR init_swapchain(
            const vk::raii::Device &logical_device,
            const vk::raii::SurfaceKHR &surface,
            const SwapchainSupport &swapchain_support,
            const QueueSupport &queue_support,
            vk::Extent2D extents) {

        vk::SwapchainCreateInfoKHR create_info = {
            .sType = vk::StructureType::eSwapchainCreateInfoKHR,
            .surface = *surface,
            .minImageCount = swapchain_support.image_count,
            .imageFormat = swapchain_support.surface_format.format,
            .imageColorSpace = swapchain_support.surface_format.colorSpace,
            .imageExtent = extents,
            .imageArrayLayers = 1,
            .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
            .preTransform = swapchain_support.surface_capabilities.currentTransform,
            .compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
            .presentMode = swapchain_support.present_mode,
            .clipped = VK_TRUE,
            .oldSwapchain = VK_NULL_HANDLE,
        };

        auto graphics_queue_index = queue_support.queue_for_feature(QueueFeature::Graphics).value();
        auto present_queue_index = queue_support.queue_for_feature(QueueFeature::Presentation).value();

        uint32_t queue_families[] = { graphics_queue_index, present_queue_index };

        if (graphics_queue_index != present_queue_index) {
            create_info.pQueueFamilyIndices = &queue_families[0];
            create_info.queueFamilyIndexCount = 2;
            create_info.imageSharingMode = vk::SharingMode::eConcurrent;
        } else {
            create_info.queueFamilyIndexCount = 0;
            create_info.imageSharingMode = vk::SharingMode::eExclusive;
        }

        return logical_device.createSwapchainKHR(create_info);
    }

    std::vector<vk::raii::ImageView> initialize_swapchain_image_views(const vk::raii::Device &logical_device, const vk::raii::SwapchainKHR &swapchain, const SwapchainSupport &swapchain_support) {
        std::vector<vk::raii::ImageView> views;
        for (const auto& image : swapchain.getImages()) {
            vk::ImageViewCreateInfo create_info = {
                .sType = vk::StructureType::eImageViewCreateInfo,
                .image = image,
                .format = swapchain_support.surface_format.format,
                .components = {
                    .r = vk::ComponentSwizzle::eR,
                    .g = vk::ComponentSwizzle::eG,
                    .b = vk::ComponentSwizzle::eB,
                    .a = vk::ComponentSwizzle::eA,
                },
                .subresourceRange = {
                    .aspectMask = vk::ImageAspectFlagBits::eColor,
                    .baseMipLevel = 0,
                    .levelCount = 1,
                    .baseArrayLayer = 0,
                    .layerCount = 1,
                },
                .viewType = vk::ImageViewType::e2D,
            };

            views.emplace_back(logical_device, create_info);
        }

        return views;
    }

    std::shared_ptr<Instance> initialize(const GraphicsInfo &info, const PlatformDelegate& delegate)
    {
        init_logger();

        auto layers = init_layers(default_validation_layers);
        auto extensions = init_extensions(info.required_extensions);

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

        // Create a surface the render pipeline will eventually render to
        auto surface = delegate.init_surface(instance);

        // Choose a physical device to use, since we only want to use one of them
        auto physical_device = select_physical_device(instance, surface);

        // Query a mix of all the queues we'll need
        auto queue_support = query_queue_support(physical_device, surface);

        // Create a logical_device
        auto logical_device = init_logical_device(physical_device, queue_support, layers);

        // Determine the size of our framebuffer
        auto framebuffer_extents = vk::Extent2D {
            .width = static_cast<uint32_t>(info.framebuffer_dimensions.x),
            .height = static_cast<uint32_t>(info.framebuffer_dimensions.y),
        };

        // Configure the swapchain support
        auto swapchain_support = query_swapchain_support(physical_device, surface);

        // Create the swapchain we will render to, using the created surface
        auto swapchain = init_swapchain(logical_device, surface, swapchain_support, queue_support, framebuffer_extents);

        // Fetch views into the swapchain images
        auto swapchain_image_views = initialize_swapchain_image_views(logical_device, swapchain, swapchain_support);

        auto instance_data = std::make_unique<Private::InstanceData>(
                std::move(context),
                std::move(instance),
                std::move(callbacks));

        return std::make_unique<Instance>(std::move(instance_data));
    }
}
