#include <Core/RenderPass.h>

namespace Core {
    void RenderPass::Submit(const RenderCommand &cmd) {
        bgfx::setVertexBuffer(0, **cmd.vertex_buffer);
        bgfx::setIndexBuffer(**cmd.index_buffer);
        bgfx::setState(cmd.state);

        // TODO: apply material properties

        bgfx::submit(0, cmd.material->GetShaderProgramHandle());
    }
}
