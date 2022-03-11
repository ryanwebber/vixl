#include <map>
#include <vector>
#include <unordered_map>

#include <VX/Graphics/Assert.h>
#include <VX/Graphics/Graphics.h>
#include <VX/Graphics/Init.h>
#include <VX/Graphics/Instance.h>
#include <VX/Graphics/Logger.h>
#include <VX/Graphics/PlatformDelegate.h>
#include <VX/Graphics/QueueSupport.h>
#include <VX/Graphics/RenderContext.h>
#include <VX/Graphics/RenderTarget.h>
#include <VX/Graphics/Swapchain.h>
#include <VX/Graphics/Vulkan.h>

#include <VX/Graphics/ExampleFragmentShader.h>
#include <VX/Graphics/ExampleVertexShader.h>

#define VX_GRAPHICS_FRAME_POOL_SIZE 2

namespace VX::Graphics {

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

    struct SwapchainSupport final {
        vk::PresentModeKHR present_mode;
        vk::SurfaceFormatKHR surface_format;
        vk::SurfaceCapabilitiesKHR surface_capabilities;
        uint32_t image_count;
    };

    static void init_debug_messenger(const vk::raii::Instance &instance, std::vector<std::shared_ptr<void>> &resources) {
        vk::DebugUtilsMessengerCreateInfoEXT create_info = {
            .messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
            .messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
            .pfnUserCallback = validation_layer_callback,
            .pUserData = nullptr,
        };

        auto messenger = instance.createDebugUtilsMessengerEXT(create_info);
        auto shared_messenger = std::make_shared<vk::raii::DebugUtilsMessengerEXT>(std::move(messenger));
        resources.push_back(std::move(shared_messenger));
    }

    static void init_callbacks(const std::vector<const char*> &extensions, const vk::raii::Instance &instance, std::vector<std::shared_ptr<void>> &extra_resources) {
        static const std::unordered_map<const char*, std::function<void(const vk::raii::Instance&, std::vector<std::shared_ptr<void>>&)>> jump_table = {
                { VK_EXT_DEBUG_UTILS_EXTENSION_NAME , init_debug_messenger },
        };

        for(const auto& extension : extensions) {
            auto entry = jump_table.find(extension);
            if (entry != jump_table.end()) {
                entry->second(instance, extra_resources);
            }
        }
    }

    static std::vector<const char*> init_layers(const std::vector<const char*> &requested_layers) {
        auto available_layers = vk::enumerateInstanceLayerProperties();
        Log::debug("Using validation layers ({}):", requested_layers.size());
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
        Log::debug("Using graphics extensions ({}):", required_extensions.size());
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

        Log::debug("Using device extensions ({}):", required_extensions.size());
        for (const auto required_extension : required_extensions) {
            Log::debug("\t- {}", required_extension);
        }

        float queue_priority = 1.0f;
        std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;
        for(const auto idx : queue_support.unique_queues()) {
            queue_create_infos.push_back({
                 .queueFamilyIndex = idx,
                 .queueCount = 1,
                 .pQueuePriorities = &queue_priority,
            });
        }

        vk::PhysicalDeviceFeatures required_features = { /* None specified */ };
        vk::DeviceCreateInfo create_info = {
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

        auto graphics_queue_index = queue_support.get_queue<QueueFeature::Graphics>();
        auto present_queue_index = queue_support.get_queue<QueueFeature::Presentation>();

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

    std::vector<vk::raii::ImageView> initialize_swapchain_image_views(const vk::raii::Device &logical_device, const SwapchainSupport &swapchain_support, const std::vector<VkImage> &images) {
        std::vector<vk::raii::ImageView> views;
        for (const auto& image : images) {
            vk::ImageViewCreateInfo create_info = {
                .image = image,
                .viewType = vk::ImageViewType::e2D,
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
            };

            views.emplace_back(logical_device, create_info);
        }

        return views;
    }

    vk::raii::ShaderModule create_shader_module(const vk::raii::Device &logical_device, const std::vector<unsigned char> &shader_binary) {
        vk::ShaderModuleCreateInfo create_info = {
            .codeSize = shader_binary.size(),
            .pCode = reinterpret_cast<const uint32_t*>(shader_binary.data()),
        };

        return logical_device.createShaderModule(create_info);
    }

    vk::raii::PipelineLayout create_pipeline_layout(const vk::raii::Device &logical_device) {
        return logical_device.createPipelineLayout({});
    }

    vk::raii::RenderPass create_render_pass(const vk::raii::Device &logical_device, vk::SurfaceFormatKHR surface_format) {
        vk::AttachmentDescription color_attachment = {
            .format = surface_format.format,
            .samples = vk::SampleCountFlagBits::e1,
            .loadOp = vk::AttachmentLoadOp::eClear,
            .storeOp = vk::AttachmentStoreOp::eStore,
            .stencilLoadOp = vk::AttachmentLoadOp::eDontCare,
            .stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
            .initialLayout = vk::ImageLayout::eUndefined,
            .finalLayout = vk::ImageLayout::ePresentSrcKHR,
        };

        // This is a direct reference to the implementation of our shader
        vk::AttachmentReference color_attachment_reference = {
            .attachment = 0,
            .layout = vk::ImageLayout::eColorAttachmentOptimal,
        };

        vk::SubpassDescription subpass_description = {
            .pipelineBindPoint = vk::PipelineBindPoint::eGraphics,
            .colorAttachmentCount = 1,
            .pColorAttachments = &color_attachment_reference,
        };

        vk::SubpassDependency image_available_dependency = {
                .srcSubpass = VK_SUBPASS_EXTERNAL,
                .dstSubpass = 0,
                .srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput,
                .dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput,
                .srcAccessMask = { },
                .dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite,
        };

        vk::RenderPassCreateInfo render_pass_create_info = {
            .attachmentCount = 1,
            .pAttachments = &color_attachment,
            .subpassCount = 1,
            .pSubpasses = &subpass_description,
            .dependencyCount = 1,
            .pDependencies = &image_available_dependency
        };

        return logical_device.createRenderPass(render_pass_create_info);
    }

    vk::raii::Pipeline create_graphics_pipeline(const vk::raii::Device &logical_device,
                                                const vk::raii::PipelineLayout &pipeline_layout,
                                                const vk::raii::RenderPass &render_pass,
                                                const vk::raii::ShaderModule &vertex_shader,
                                                const vk::raii::ShaderModule &fragment_shader,
                                                vk::Extent2D extents) {

        vk::PipelineVertexInputStateCreateInfo vertex_input_state_create_info = { };

        vk::PipelineInputAssemblyStateCreateInfo input_assembly_state_create_info = {
            .topology = vk::PrimitiveTopology::eTriangleList,
            .primitiveRestartEnable = false,
        };

        vk::Viewport viewport = {
            .x = 0.0f,
            .y = 0.0f,
            .width = static_cast<float>(extents.width),
            .height = static_cast<float>(extents.height),
            .minDepth = 0.0f,
            .maxDepth = 1.0f,
        };

        vk::Rect2D scissor = {
            .offset = { 0, 0 },
            .extent = extents,
        };

        vk::PipelineViewportStateCreateInfo viewport_state_create_info = {
            .viewportCount = 1,
            .pViewports = &viewport,
            .scissorCount = 1,
            .pScissors = &scissor,
        };

        vk::PipelineRasterizationStateCreateInfo rasterization_state_create_info = {
            .depthClampEnable = false,
            .rasterizerDiscardEnable = false,
            .polygonMode = vk::PolygonMode::eFill,
            .cullMode = vk::CullModeFlagBits::eBack,
            .frontFace = vk::FrontFace::eClockwise,
                .depthBiasEnable = false,
            .lineWidth = 1.0f,
        };

        vk::PipelineMultisampleStateCreateInfo multisample_state_create_info = {
            .rasterizationSamples = vk::SampleCountFlagBits::e1,
            .sampleShadingEnable = false,
        };

        vk::PipelineColorBlendAttachmentState color_blend_attachment_state = {
            .blendEnable = true,
            .srcColorBlendFactor = vk::BlendFactor::eSrcAlpha,
            .dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha,
            .colorBlendOp = vk::BlendOp::eAdd,
            .srcAlphaBlendFactor = vk::BlendFactor::eOne,
            .dstAlphaBlendFactor = vk::BlendFactor::eZero,
            .alphaBlendOp = vk::BlendOp::eAdd,
        };

        vk::PipelineColorBlendStateCreateInfo color_blend_state_create_info = {
            .logicOpEnable = false,
            .logicOp = vk::LogicOp::eCopy,
            .attachmentCount = 1,
            .pAttachments = &color_blend_attachment_state,
        };

        vk::PipelineShaderStageCreateInfo shader_stages[] = {
            {
                .stage = vk::ShaderStageFlagBits::eVertex,
                .module = *vertex_shader,
                .pName = "main",
            },
            {
                .stage = vk::ShaderStageFlagBits::eFragment,
                .module = *fragment_shader,
                .pName = "main",
            },
        };

        vk::GraphicsPipelineCreateInfo pipeline_create_info = {
            .stageCount = 2,
            .pStages = &shader_stages[0],
            .pVertexInputState = &vertex_input_state_create_info,
            .pInputAssemblyState = &input_assembly_state_create_info,
            .pViewportState = &viewport_state_create_info,
            .pRasterizationState = &rasterization_state_create_info,
            .pMultisampleState = &multisample_state_create_info,
            .pDepthStencilState = nullptr,
            .pColorBlendState = &color_blend_state_create_info,
            .pDynamicState = nullptr,

            .layout = *pipeline_layout,
            .renderPass = *render_pass,
            .subpass = 0,
        };

        return logical_device.createGraphicsPipeline({ nullptr }, pipeline_create_info);
    }

    std::vector<vk::raii::Framebuffer> create_framebuffers(
            const vk::raii::Device &logical_device,
            const vk::raii::RenderPass &render_pass,
            const std::vector<vk::raii::ImageView> &images,
            vk::Extent2D extents) {

        std::vector<vk::raii::Framebuffer> framebuffers;
        for (const auto& image : images) {
            auto image_ref = *image;
            vk::FramebufferCreateInfo create_info = {
                .renderPass = *render_pass,
                .attachmentCount = 1,
                .pAttachments = &image_ref,
                .width = extents.width,
                .height = extents.height,
                .layers = 1,
            };

            framebuffers.emplace_back(logical_device, create_info);
        }

        return framebuffers;
    }

    vk::raii::CommandPool create_command_pool(const vk::raii::Device &logical_device, const QueueSupport &queue_support) {
        vk::CommandPoolCreateInfo create_info = {
            .queueFamilyIndex = queue_support.get_queue<QueueFeature::Graphics>(),
        };

        return logical_device.createCommandPool(create_info);
    }

    std::vector<vk::raii::CommandBuffer> create_command_buffers(const vk::raii::Device &logical_device, const vk::raii::CommandPool &command_pool, size_t buffer_count) {
        vk::CommandBufferAllocateInfo allocate_info = {
            .commandPool = *command_pool,
            .level = vk::CommandBufferLevel::ePrimary,
            .commandBufferCount = static_cast<uint32_t >(buffer_count),
        };

        return logical_device.allocateCommandBuffers(allocate_info);
    }

    Instance try_init(const GraphicsInfo &info, const PlatformDelegate& delegate)
    {
        init_logger();

        auto layers = init_layers(default_validation_layers);
        auto extensions = init_extensions(info.required_extensions);

        vk::raii::Context context;

        vk::ApplicationInfo app_info = {
                .pApplicationName = "Generic Vixl Application",
                .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
                .pEngineName = "Vixl",
                .engineVersion = VK_MAKE_VERSION(1, 0, 0),
                .apiVersion = VK_API_VERSION_1_2,
        };

        vk::InstanceCreateInfo instance_info = {
                .pApplicationInfo = &app_info,

                .enabledLayerCount = static_cast<uint32_t>(layers.size()),
                .ppEnabledLayerNames = layers.data(),

                .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
                .ppEnabledExtensionNames = extensions.data(),
        };

        vk::raii::Instance instance(context, instance_info);

        std::vector<std::shared_ptr<void>> callbacks;

        // Setup extension callbacks (ex. debug messaging)
        init_callbacks(extensions, instance, callbacks);

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
        auto swapchain_image_views = initialize_swapchain_image_views(logical_device, swapchain_support, swapchain.getImages());

        auto render_pass = create_render_pass(logical_device, swapchain_support.surface_format);
        auto framebuffers = create_framebuffers(logical_device, render_pass, swapchain_image_views, framebuffer_extents);
        auto command_pool = create_command_pool(logical_device, queue_support);
        auto command_buffers = create_command_buffers(logical_device, command_pool, VX_GRAPHICS_FRAME_POOL_SIZE);

        // TODO: this can probably be factored better

        auto render_target_allocator = ResourceAllocator<RenderTarget, HandleType::RenderTarget>::create_shared();
        auto render_context_allocator = ResourceAllocator<RenderContext, HandleType::RenderContext>::create_shared();
        auto command_buffer_allocator = ResourceAllocator<vk::raii::CommandBuffer, HandleType::CommandBuffer>::create_shared();
        auto graphics_pipeline_allocator = ResourceAllocator<vk::raii::Pipeline, HandleType::GraphicsPipeline>::create_shared();

        std::vector<SwapFrame> swap_frames;

        VX_GRAPHICS_ASSERT(swapchain_image_views.size() == framebuffers.size(), "Expected framebuffers to correspond to the swapchain views");
        for (auto i = 0; i < framebuffers.size(); i++) {
            auto& image_view = swapchain_image_views[i];
            auto& framebuffer = framebuffers[i];
            auto render_target_handle = render_target_allocator->create_resource([&](){
                return RenderTarget(std::move(image_view), std::move(framebuffer), framebuffer_extents);
            });

            vk::SemaphoreCreateInfo semaphore_create_info = { };
            vk::FenceCreateInfo fence_create_info = { .flags = vk::FenceCreateFlagBits::eSignaled };
            auto render_fence = logical_device.createFence(fence_create_info);
            auto wait_semaphore = logical_device.createSemaphore(semaphore_create_info);
            auto signal_semaphore = logical_device.createSemaphore(semaphore_create_info);
            auto render_context_handle = render_context_allocator->create_resource([&](){
               return RenderContext(std::move(render_fence),
                                    std::move(wait_semaphore),
                                    std::move(signal_semaphore));
            });

            swap_frames.emplace_back(std::move(render_target_handle), std::move(render_context_handle));
        }

        std::vector<SwapBuffer> swap_buffers;
        for (auto &command_buffer : command_buffers) {
            auto command_buffer_handle = command_buffer_allocator->create_resource(std::move(command_buffer));
            swap_buffers.emplace_back(std::move(command_buffer_handle));
        }

        Swapchain swapchain_wrapper(std::move(swapchain),
                                    swapchain_support.present_mode,
                                    swapchain_support.surface_format,
                                    swapchain_support.surface_capabilities,
                                    std::move(swap_frames),
                                    std::move(swap_buffers));

        auto instance_impl = std::make_shared<InstanceImpl>(std::move(context),
                                                            std::move(instance),
                                                            std::move(callbacks),
                                                            std::move(surface),
                                                            std::move(physical_device),
                                                            std::move(logical_device),
                                                            std::move(swapchain_wrapper),
                                                            std::move(render_pass),
                                                            std::move(command_pool),
                                                            std::move(render_target_allocator),
                                                            std::move(render_context_allocator),
                                                            std::move(command_buffer_allocator),
                                                            std::move(graphics_pipeline_allocator),
                                                            std::move(queue_support));

        return Instance(std::move(instance_impl));
    }

    VX::Expected<Instance> init(const GraphicsInfo &info, const PlatformDelegate& delegate) {
        try {
            return try_init(info, delegate);
        } catch (const vk::Error &error) {
            return VX::make_unexpected("Init: {}", error.what());
        }

        VX_GRAPHICS_ASSERT_NOT_REACHED();
    }
}
