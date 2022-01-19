#include <bx/math.h>
#include <glm/gtc/type_ptr.hpp>

#include <VX/Core/RenderPass.h>
#include <VX/Core/SceneRenderer.h>
#include <VX/Core/Logger.h>

namespace VX::Core {

    void SceneRenderer::OnInitialize() {
        if (m_Bultins == nullptr)
            m_Bultins = std::make_shared<RenderBuiltins>();
    }

    void SceneRenderer::OnDestroy() {
    }

    void SceneRenderer::OnUpdate() {
        auto it = std::remove_if(m_Contexts.begin(), m_Contexts.end(), [](const std::weak_ptr<RenderContext>& context) {
            return context.expired();
        });

        m_Contexts.erase(it, m_Contexts.end());
    }

    void SceneRenderer::OnRender() {
        RenderPass rp;
        for (auto&& weak_context : m_Contexts) {
            if (!weak_context.expired()) {
                auto context = weak_context.lock();

                auto view_matrix_ptr = glm::value_ptr(context->GetViewProjection().view_matrix);
                auto proj_matrix_ptr = glm::value_ptr(context->GetViewProjection().projection_matrix);
                bgfx::setViewTransform(0, view_matrix_ptr, proj_matrix_ptr);

                for (auto &command : context->GetBuffer().GetCommands()) {
                    rp.Submit(command);
                }

                // Wipe all commands and reset for the next frame
                context->Reset();
            }
        }
    }

    std::shared_ptr<RenderContext> SceneRenderer::CreateRenderContext() {
        auto target = std::make_shared<RenderContext>(m_Bultins);
        m_Contexts.push_back(target);
        return target;
    }
}
