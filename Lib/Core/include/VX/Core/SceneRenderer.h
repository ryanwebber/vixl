#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <VX/Noncopyable.h>

#include <VX/Core/RenderContext.h>
#include <VX/Core/RenderLayer.h>
#include <VX/Core/RenderBuiltins.h>

namespace VX::Core {
    class SceneRenderer final : public RenderLayer {
        VX_MAKE_NONMOVABLE(SceneRenderer);
        VX_MAKE_NONCOPYABLE(SceneRenderer);
    private:
        std::shared_ptr<RenderBuiltins> m_Bultins;
        std::vector<std::weak_ptr<RenderContext>> m_Contexts;

    public:
        explicit SceneRenderer() = default; // TODO: figure out how to load builtins

        ~SceneRenderer() override = default;

        void OnInitialize() override;
        void OnDestroy() override;
        void OnUpdate() override;
        void OnRender() override;

        std::shared_ptr<RenderContext> CreateRenderContext();
    };
}
