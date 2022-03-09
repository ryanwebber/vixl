#pragma once

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

#include <VX/Graphics/Vulkan.h>

namespace VX::Graphics {
    class RenderContext final {
        VX_MAKE_NONCOPYABLE(RenderContext);
        VX_DEFAULT_MOVABLE(RenderContext);
    private:
        vk::raii::Fence m_render_fence;
        vk::raii::Semaphore m_wait_semaphore;
        vk::raii::Semaphore m_signal_semaphore;
    public:
        RenderContext(vk::raii::Fence render_fence,
                      vk::raii::Semaphore wait_semaphore,
                      vk::raii::Semaphore signal_semaphore)
                : m_render_fence(std::move(render_fence))
                , m_wait_semaphore(std::move(wait_semaphore))
                , m_signal_semaphore(std::move(signal_semaphore))
        {}

        ~RenderContext() = default;
    };
}
