#include <VX/Graphics/Instance.h>
#include <VX/Graphics/GraphicsContextImpl.h>

namespace VX::Graphics {

    void GraphicsContextImpl::bind_program(const GraphicsProgramHandle &handle)
    {
         const auto &graphics_program = handles().graphics_program.lookup(handle);
         command_buffer().bindPipeline(vk::PipelineBindPoint::eGraphics, *graphics_program.pipeline());
    }

    void GraphicsContextImpl::draw(uint32_t vertex_count)
    {
        command_buffer().draw(vertex_count, 1, 0, 0);
    }
}
