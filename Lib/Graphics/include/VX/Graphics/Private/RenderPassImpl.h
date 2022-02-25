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

    public:
        RenderPassImpl(std::shared_ptr<vk::raii::RenderPass> render_pass, std::shared_ptr<CommandBuffer> command_buffer)
            : m_render_pass(std::move(render_pass))
            , m_command_recorder(std::move(command_buffer))
        {}

        [[nodiscard]] const CommandRecorder& command_recorder() const { return m_command_recorder; }
        CommandRecorder& command_recorder() { return m_command_recorder; }
    };
}
