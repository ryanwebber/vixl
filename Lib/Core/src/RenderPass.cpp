#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Core/RenderPass.h>

namespace Core {
    void RenderPass::Submit(const RenderCommand &cmd) {
        bgfx::setVertexBuffer(0, cmd.GetVertexBuffer().Get());
        bgfx::setIndexBuffer(cmd.GetIndexBuffer().Get());
        bgfx::setState(cmd.GetState());
        bgfx::setTransform(glm::value_ptr(cmd.GetModelTransform()));

        // TODO: apply material properties

        bgfx::submit(0, cmd.GetMaterial().GetShaderProgramHandle());
    }
}
