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
        RenderTarget m_render_target;
        RenderTiming m_render_timing;
        CommandRecorder m_command_recorder;
        QueueSupport m_queue_support;
        std::shared_ptr<vk::raii::Device> m_device;
        std::shared_ptr<vk::raii::RenderPass> m_render_pass;

    public:
        RenderPassImpl(RenderTarget render_target,
                       RenderTiming render_timing,
                       CommandRecorder command_recorder,
                       QueueSupport queue_support,
                       std::shared_ptr<vk::raii::Device> device,
                       std::shared_ptr<vk::raii::RenderPass> render_pass)
            : m_render_target(std::move(render_target))
            , m_render_timing(std::move(render_timing))
            , m_command_recorder(std::move(command_recorder))
            , m_queue_support(std::move(queue_support))
            , m_device(std::move(device))
            , m_render_pass(std::move(render_pass))
        {}

        CommandRecorder& command_recorder() { return m_command_recorder; }
        [[nodiscard]] const CommandRecorder& command_recorder() const { return m_command_recorder; }

        void submit();
    };
}
