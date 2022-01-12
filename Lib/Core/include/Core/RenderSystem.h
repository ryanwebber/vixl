#pragma once

#include <memory>

#include <entt/entity/registry.hpp>

#include <Core/RenderBuffer.h>
#include <Core/SceneCamera.h>

namespace Core {
    class RenderSystem {
    VX_MAKE_NONMOVABLE(RenderSystem);
    VX_MAKE_NONCOPYABLE(RenderSystem);

    public:
        RenderSystem() = default;
        virtual ~RenderSystem() = default;

        virtual void Render(RenderBuffer&, entt::registry&) = 0;
    };
}
