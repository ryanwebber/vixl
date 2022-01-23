#pragma once

#include <VX/Noncopyable.h>

#include <VX/Core/RenderBuiltins.h>
#include <VX/Core/RenderSystem.h>
#include <VX/Core/Types.h>

namespace VX::Core::System {
    class SpriteRenderingSystem final : public RenderSystem {
        VX_MAKE_NONCOPYABLE(SpriteRenderingSystem);
        VX_MAKE_NONMOVABLE(SpriteRenderingSystem);
    private:
        Shape m_quad_shape;
    public:
        explicit SpriteRenderingSystem(const RenderBuiltins&);
        ~SpriteRenderingSystem() override = default;

        void render(RenderBuffer&, entt::registry& entities) override;
    };
}
