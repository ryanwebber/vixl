#include <bx/math.h>
#include <Core/RenderPass.h>
#include <Core/SceneRenderer.h>
#include <Core/Logger.h>

namespace Core {

    void SceneRenderer::OnInitialize() {
    }

    void SceneRenderer::OnDestroy() {
    }

    void SceneRenderer::OnUpdate() {
        auto it = std::remove_if(m_Targets.begin(), m_Targets.end(), [](const std::weak_ptr<RenderTarget>& target) {
            return target.expired();
        });

        m_Targets.erase(it, m_Targets.end());
    }

    void SceneRenderer::OnRender() {
        const bx::Vec3 at = {0.0f, 0.0f,  0.0f};
        const bx::Vec3 eye = {0.0f, 0.0f, -5.0f};
        float view[16];
        bx::mtxLookAt(view, eye, at);
        float proj[16];
        bx::mtxProj(proj, 60.0f, (800.0f / 600.0f), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
        bgfx::setViewTransform(0, view, proj);

        RenderPass rp;
        for (auto&& weak_target : m_Targets) {
            if (!weak_target.expired()) {
                auto target = weak_target.lock();
                for (auto &command : target->GetCommands()) {
                    rp.Submit(command);
                }

                // Wipe all commands and reset for the next frame
                target->Reset();
            }
        }
    }

    std::shared_ptr<RenderTarget> SceneRenderer::CreateRenderTarget() {
        auto target_ptr = new RenderTarget();
        auto target = std::shared_ptr<RenderTarget>(target_ptr);
        m_Targets.push_back(target);
        return std::move(target);
    }
}
