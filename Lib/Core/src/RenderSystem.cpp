#include <Core/RenderSystem.h>

namespace Core {
    RenderSystem::RenderSystem(std::shared_ptr<RenderService> render_service)
        : m_RenderService(std::move(render_service))
        {
        }

    void RenderSystem::Update() {
    }
}
