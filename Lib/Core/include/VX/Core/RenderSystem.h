#pragma once

#include <memory>

#include <entt/entity/registry.hpp>

#include <VX/Core/RenderBuffer.h>
#include <VX/Core/SceneCamera.h>

namespace VX::Core {
    class RenderSystem {
    VX_MAKE_NONMOVABLE(RenderSystem);
    VX_MAKE_NONCOPYABLE(RenderSystem);

    public:
        RenderSystem() = default;
        virtual ~RenderSystem() = default;

        virtual void Render(RenderBuffer&, entt::registry&) = 0;
    };
}
