#include <Core/SceneRenderer.h>

namespace Core {

    void SceneRenderer::OnInitialize() {
    }

    void SceneRenderer::OnDestroy() {
    }

    void SceneRenderer::OnUpdate() {
        auto it = std::remove_if(m_Cameras.begin(), m_Cameras.end(), [](std::weak_ptr<RenderCamera> cam) {
            return cam.expired();
        });

        m_Cameras.erase(it, m_Cameras.end());
    }

    void SceneRenderer::OnRender() {
        RenderPass rp;
        for (auto&& weak_rc : m_Cameras) {
            if (weak_rc.expired()) {
                auto rc = weak_rc.lock();
                for (auto&& command : rc->GetRenderCommands()) {
                    command->Render(rp);
                }
            }
        }
    }

    std::shared_ptr<RenderCamera> SceneRenderer::CreateRenderCamera() {
        auto camera_ptr = new RenderCamera();
        auto camera = std::shared_ptr<RenderCamera>(camera_ptr);
        m_Cameras.push_back(camera);
        return std::move(camera);
    }
}
