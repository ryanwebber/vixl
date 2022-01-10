#include <bx/math.h>
#include <glm/gtc/type_ptr.hpp>

#include <Core/RenderPass.h>
#include <Core/SceneRenderer.h>
#include <Core/Logger.h>

namespace Core {

    void SceneRenderer::OnInitialize() {
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

                // TODO: Remove this in favor of the GLM matrix in the render context
                const bx::Vec3 at = {0.0f, 0.0f,  0.0f};
                const bx::Vec3 eye = {0.0f, 0.0f, -5.0f};
                float view[16];
                bx::mtxLookAt(view, eye, at);
                float proj[16];
                bx::mtxProj(proj, 80.0f, (800.0f / 600.0f), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
                bgfx::setViewTransform(0, view, proj);

                for (auto &command : context->GetBuffer().GetCommands()) {
                    rp.Submit(command);
                }

                // Wipe all commands and reset for the next frame
                context->Reset();
            }
        }
    }

    std::shared_ptr<RenderContext> SceneRenderer::CreateRenderContext() {
        auto target = std::make_shared<RenderContext>();
        m_Contexts.push_back(target);
        return std::move(target);
    }
}
