#pragma once

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

#include <VX/Graphics/CommandBuffer.h>

namespace VX::Graphics {

    class CommandRecorder final {
        VX_DEFAULT_COPYABLE(CommandRecorder);
        VX_DEFAULT_MOVABLE(CommandRecorder);
    private:
        std::shared_ptr<CommandBuffer> m_command_buffer;

    public:
        explicit CommandRecorder(std::shared_ptr<CommandBuffer> command_buffer)
            : m_command_buffer(std::move(command_buffer))
        {}

        ~CommandRecorder() = default;

        CommandBuffer &command_buffer() { return *m_command_buffer; };
        [[nodiscard]] const CommandBuffer &command_buffer() const { return *m_command_buffer; };

        // void BindGraphicsPipeline()
        // void Draw(...)
        // void DrawInstanced()
        // void Submit()
    };
}
