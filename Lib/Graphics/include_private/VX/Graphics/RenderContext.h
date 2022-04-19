#pragma once

#include <span>

#include <VX/Graphics/Memory.h>
#include <VX/Graphics/Vulkan.h>

namespace VX::Graphics {
    struct RenderContext final {
        InstancePtr<vk::raii::CommandBuffer> command_buffer;
        InstancePtr<vk::raii::Fence> m_signal_fence;
        std::span<InstancePtr<vk::raii::Semaphore>> m_wait_semaphores { };
        std::span<InstancePtr<vk::raii::Semaphore>> m_signal_semaphores { };
    };
}
