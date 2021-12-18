#pragma once

#include <vector>
#include <memory>

#include <Core/RenderCamera.h>

namespace Core {
    class RenderService final {
    private:
        std::vector<std::shared_ptr<RenderCamera>> m_Cameras;
    public:
        RenderService() = default;
        ~RenderService() = default;

        void AddCamera(std::shared_ptr<RenderCamera>);
    };
}
