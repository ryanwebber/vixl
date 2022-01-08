#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <Common/Noncopyable.h>

#include <Core/RenderTarget.h>
#include <Core/RenderLayer.h>

namespace Core {
    class SceneRenderer final : public RenderLayer {
        VX_MAKE_NONMOVABLE(SceneRenderer);
        VX_MAKE_NONCOPYABLE(SceneRenderer);
    private:
        std::vector<std::weak_ptr<RenderTarget>> m_Targets;

    public:
        SceneRenderer() = default;
        ~SceneRenderer() override = default;

        void OnInitialize() override;
        void OnDestroy() override;
        void OnUpdate() override;
        void OnRender() override;

        std::shared_ptr<RenderTarget> CreateRenderTarget();
    };
}
