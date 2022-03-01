#pragma once

#include <VX/Graphics/APIObject.h>
#include <VX/Graphics/Private/Vulkan.h>

namespace VX::Graphics::Private {
    class CommandBufferImpl : public APIWrapper<vk::raii::CommandBuffer> {
        using APIWrapper<vk::raii::CommandBuffer>::APIWrapper;
    };

    class FramebufferImpl : public APIWrapper<vk::raii::Framebuffer> {
        using APIWrapper<vk::raii::Framebuffer>::APIWrapper;
    };

    class SemaphoreImpl : public APIWrapper<vk::raii::Semaphore> {
        using APIWrapper<vk::raii::Semaphore>::APIWrapper;
    };

    class FenceImpl : public APIWrapper<vk::raii::Fence> {
        using APIWrapper<vk::raii::Fence>::APIWrapper;
    };
}
