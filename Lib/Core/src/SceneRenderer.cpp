#include <bx/math.h>
#include <glm/gtc/type_ptr.hpp>

#include <Core/RenderPass.h>
#include <Core/SceneRenderer.h>
#include <Core/Logger.h>

namespace Core {

    void SceneRenderer::OnInitialize() {
        if (m_Primitives == nullptr)
            m_Primitives = std::make_shared<RenderPrimitives>();
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
        auto target = std::make_shared<RenderContext>(m_Primitives);
        m_Contexts.push_back(target);
        return std::move(target);
    }
}
