#pragma once

#include <array>
#include <optional>

#include <VX/Graphics/GraphicsProgram.h>
#include <VX/Graphics/Memory.h>
#include <VX/Graphics/Vulkan.h>

namespace VX::Graphics {

    constexpr auto k_max_graphics_pipeline_semaphores = 4;
    using SemaphoreBuffer = std::array<std::optional<InstancePtr<vk::raii::Semaphore>>, k_max_graphics_pipeline_semaphores>;

    struct RenderContext final {
        InstancePtr<vk::raii::CommandBuffer> command_buffer;
        InstancePtr<vk::raii::Fence> signal_fence;
        SemaphoreBuffer wait_semaphores { };
        SemaphoreBuffer signal_semaphores { };
    };
}
