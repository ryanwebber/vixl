#pragma once

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

#include <VX/Graphics/Private/Vulkan.h>

namespace VX::Graphics::Private {
    class CommandBufferImpl final {
        VX_MAKE_NONCOPYABLE(CommandBufferImpl);
        VX_DEFAULT_MOVABLE(CommandBufferImpl);
    private:
        vk::raii::CommandBuffer m_command_buffer;

    public:
        explicit CommandBufferImpl(vk::raii::CommandBuffer command_buffer)
            : m_command_buffer(std::move(command_buffer))
        {}

        vk::raii::CommandBuffer &command_buffer() { return m_command_buffer; }
        [[nodiscard]] const vk::raii::CommandBuffer &command_buffer() const { return m_command_buffer; }
    };
}
