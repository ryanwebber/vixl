#pragma once

#include <memory>

#include <Core/RenderCamera.h>
#include <Core/SceneCamera.h>

namespace Core {
    class RenderSystem {
    VX_MAKE_NONMOVABLE(RenderSystem);
    VX_MAKE_NONCOPYABLE(RenderSystem);

    public:
        RenderSystem() = default;
        virtual ~RenderSystem() = default;

        virtual void Render(RenderCamera& target, SceneCamera const &camera) = 0;
    };
}
