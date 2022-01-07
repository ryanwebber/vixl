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
        for (auto&& weak_rc : m_Targets) {
            if (weak_rc.expired()) {
                auto rc = weak_rc.lock();
                for (auto&& command : rc->GetRenderCommands()) {
                    command->Render(rp);
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
