#include <VX/Graphics/CommandRecorder.h>
#include <VX/Graphics/Private/CommandRecorderImpl.h>
#include <VX/Graphics/Private/GraphicsPipelineImpl.h>
#include <VX/Graphics/Private/Vulkan.h>
#include <VX/Graphics/Private/Wrappers.h>

namespace VX::Graphics {
    namespace Private {
        void CommandRecorderImpl::bind(const GraphicsPipeline &graphics_pipeline) const {
            m_command_buffer->impl()->bindPipeline(vk::PipelineBindPoint::eGraphics, *graphics_pipeline->pipeline());
        }

        void CommandRecorderImpl::draw() const {
            m_command_buffer->impl()->draw(3, 1, 0, 0); // TODO: obviously wrong
        }
    }

    void CommandRecorder::bind(const GraphicsPipeline &graphics_pipeline) const {
        impl().bind(graphics_pipeline);
    }

    void CommandRecorder::draw() const {
        impl().draw();
    }
}
