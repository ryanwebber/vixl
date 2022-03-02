#pragma once

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

#include <VX/Graphics/APIObject.h>
#include <VX/Graphics/Framebuffer.h>
#include <VX/Graphics/RenderTarget.h>
#include <VX/Graphics/Private/QueueSupport.h>
#include <VX/Graphics/Private/Vulkan.h>

namespace VX::Graphics::Private {

    class RenderPassImpl final {
        VX_MAKE_NONCOPYABLE(RenderPassImpl);
        VX_DEFAULT_MOVABLE(RenderPassImpl);
    private:
        CommandRecorder m_command_recorder;
        RenderTarget m_render_target;
        std::shared_ptr<Fence> m_render_fence;
        std::shared_ptr<Semaphore> m_wait_semaphore;
        std::shared_ptr<Semaphore> m_signal_semaphore;

    public:
        RenderPassImpl(CommandRecorder command_recorder,
                       RenderTarget render_target,
                       std::shared_ptr<Fence> render_fence,
                       std::shared_ptr<Semaphore> wait_semaphore,
                       std::shared_ptr<Semaphore> signal_semaphore)
            : m_command_recorder(std::move(command_recorder))
            , m_render_target(std::move(render_target))
            , m_render_fence(std::move(render_fence))
            , m_wait_semaphore(std::move(wait_semaphore))
            , m_signal_semaphore(std::move(signal_semaphore))
        {}

        CommandRecorder& command_recorder() { return m_command_recorder; }
        [[nodiscard]] const CommandRecorder& command_recorder() const { return m_command_recorder; }

        RenderTarget &render_target() { return m_render_target; }
        [[nodiscard]] const RenderTarget &render_target() const { return m_render_target; }

        Fence &render_fence() { return *m_render_fence; }
        [[nodiscard]] const Fence &render_fence() const { return *m_render_fence; }

        Semaphore &wait_semaphore() { return *m_wait_semaphore; }
        [[nodiscard]] const Semaphore &wait_semaphore() const { return *m_wait_semaphore; }

        Semaphore &signal_semaphore() { return *m_signal_semaphore; }
        [[nodiscard]] const Semaphore &signal_semaphore() const { return *m_signal_semaphore; }
    };
}
