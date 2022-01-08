#include <Core/RenderPass.h>
#include <Core/SceneRenderer.h>

namespace Core {

    void SceneRenderer::OnInitialize() {
    }

    void SceneRenderer::OnDestroy() {
    }

    void SceneRenderer::OnUpdate() {
        auto it = std::remove_if(m_Targets.begin(), m_Targets.end(), [](const std::weak_ptr<RenderTarget>& cam) {
            return cam.expired();
        });

        m_Targets.erase(it, m_Targets.end());
    }

    void SceneRenderer::OnRender() {
        RenderPass rp;
        for (auto&& weak_target : m_Targets) {
            if (!weak_target.expired()) {
                auto target = weak_target.lock();
                target->Reset();
                for (auto&& command : target->Commands()) {
                    rp.Submit(command);
                }
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
