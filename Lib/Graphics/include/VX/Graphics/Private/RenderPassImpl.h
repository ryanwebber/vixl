#pragma once

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

#include <VX/Graphics/APIObject.h>
#include <VX/Graphics/Framebuffer.h>
#include <VX/Graphics/Private/Vulkan.h>

namespace VX::Graphics::Private {

    class RenderPassImpl final {
        VX_MAKE_NONCOPYABLE(RenderPassImpl);
        VX_DEFAULT_MOVABLE(RenderPassImpl);
    private:
        CommandRecorder m_command_recorder;
        std::shared_ptr<vk::raii::RenderPass> m_render_pass;
        std::shared_ptr<Framebuffer> m_framebuffer;

    public:
        RenderPassImpl(std::shared_ptr<vk::raii::RenderPass> render_pass, std::shared_ptr<Framebuffer> framebuffer)
            : m_render_pass(std::move(render_pass))
            , m_command_recorder(framebuffer->associated_command_buffer())
            , m_framebuffer(std::move(framebuffer))
        {}

        [[nodiscard]] const CommandRecorder& command_recorder() const { return m_command_recorder; }
        CommandRecorder& command_recorder() { return m_command_recorder; }
    };
}
