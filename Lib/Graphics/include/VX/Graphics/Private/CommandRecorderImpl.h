#pragma once

#include <memory>

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

#include <VX/Graphics/CommandBuffer.h>
#include <VX/Graphics/CommandRecorder.h>
#include <VX/Graphics/GraphicsPipeline.h>

namespace VX::Graphics::Private {
    class CommandRecorderImpl final {
        VX_DEFAULT_MOVABLE(CommandRecorderImpl);
        VX_MAKE_NONCOPYABLE(CommandRecorderImpl);
    private:
        std::shared_ptr<CommandBuffer> m_command_buffer;
    public:
        explicit CommandRecorderImpl(std::shared_ptr<CommandBuffer> command_buffer)
            : m_command_buffer(std::move(command_buffer))
        {}

        ~CommandRecorderImpl() = default;

        CommandBuffer &command_buffer() { return *m_command_buffer; }
        [[nodiscard]] const CommandBuffer &command_buffer() const { return *m_command_buffer; }

        void bind(const GraphicsPipeline&) const;
        void draw() const;
    };
}
