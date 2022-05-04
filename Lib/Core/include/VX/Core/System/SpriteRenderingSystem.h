#pragma once

#include <VX/Noncopyable.h>

#include <VX/Core/RenderBuiltins.h>
#include <VX/Core/RenderSystem.h>
#include <VX/Core/SceneContext.h>

namespace VX::Core::System {
    class SpriteRenderingSystem final : public RenderSystem {
        VX_MAKE_NONCOPYABLE(SpriteRenderingSystem);
        VX_MAKE_NONMOVABLE(SpriteRenderingSystem);
    private:
    public:
        explicit SpriteRenderingSystem();
        ~SpriteRenderingSystem() override = default;

        void render(const SceneContext&, const RenderContext&, RenderPass&, const entt::registry&) override;
    };
}
