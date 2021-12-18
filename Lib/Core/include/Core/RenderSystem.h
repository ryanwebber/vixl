#pragma once

#include <memory>

#include <Core/RenderService.h>
#include <Core/UpdateSystem.h>

namespace Core {
    class RenderSystem final : public UpdateSystem {
    private:
        std::shared_ptr<RenderService> m_RenderService;

    public:
        explicit RenderSystem(std::shared_ptr<RenderService>);
        ~RenderSystem() override = default;

        void Update() override;
    };
}
