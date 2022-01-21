#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <VX/Core/RenderPass.h>

namespace VX::Core {
    void RenderPass::submit(const RenderCommand &cmd) {
        bgfx::setVertexBuffer(0, *cmd.vertex_buffer());
        bgfx::setIndexBuffer(*cmd.index_buffer());
        bgfx::setState(cmd.state());
        bgfx::setTransform(glm::value_ptr(cmd.transform()));

        for (const auto& [slot, sample_texture] : cmd.material().texture_slots()) {
            bgfx::setTexture(slot, **sample_texture.sampler_handle, **sample_texture.texture_handle);
        }

        bgfx::submit(0, cmd.material().shader_program_handle());
    }
}
