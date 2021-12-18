#include <Core/RenderService.h>

namespace Core {
    void RenderService::AddCamera(std::shared_ptr<RenderCamera> camera) {
        m_Cameras.push_back(std::move(camera));
    }
}
